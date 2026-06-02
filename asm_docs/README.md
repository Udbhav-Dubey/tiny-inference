# SIMD Investigation 1 — GCC Auto-Vectorization

## Goal

Determine whether GCC is already auto-vectorizing the GEMM kernels under the default Release build, without any manual intrinsics.

---

## Method

Generated assembly from the compiled GEMM code:

```bash
g++ -O3 -std=c++20 -S GEMM.cpp -o GEMM.s
```

Inspected for SIMD instruction patterns:

```bash
grep -i ymm GEMM.s    # AVX 256-bit registers
grep -i xmm GEMM.s    # SSE 128-bit registers
grep -i vfmadd GEMM.s # Fused Multiply-Add
grep -i mulss GEMM.s  # scalar float multiply
```

---

## Findings Per Kernel

### `Gemm_ikj` — flat ikj kernel

The inner `j` loop is vectorized. GCC broadcasts `ak` into all four lanes of an XMM register, then processes 4 floats per iteration:

```asm
movaps  %xmm2, %xmm1
shufps  $0, %xmm1, %xmm1   ; broadcast ak into all 4 lanes

.L8:
movups  (%r8,%rsi), %xmm0   ; load 4 B values
movups  (%rax,%rsi), %xmm3  ; load 4 C values
mulps   %xmm1, %xmm0        ; multiply 4 floats
addps   %xmm3, %xmm0        ; add 4 floats
movups  %xmm0, (%rax,%rsi)  ; store 4 floats
addq    $16, %rsi
cmpq    %rsi, %rdx
jne     .L8

; remaining elements handled scalar:
.L14:
movss   (%r8,%rsi), %xmm0
mulss   %xmm2, %xmm0
addss   (%rax,%rsi), %xmm0
movss   %xmm0, (%rax,%rsi)
```

4 floats processed per SIMD iteration. Remainder handled scalar.

---

### `Gemm_ijk` — flat ijk kernel

GCC generates a SIMD multiply followed by a horizontal reduction back to scalar:

```asm
movups  -16(%r9), %xmm4     ; load 4 values
mulps   %xmm4, %xmm0        ; SIMD multiply

; horizontal reduction:
addss   %xmm0, %xmm1
movaps  %xmm0, %xmm2
shufps  $85, %xmm0, %xmm2
addss   %xmm1, %xmm2
movaps  %xmm0, %xmm1
unpckhps %xmm0, %xmm1
shufps  $255, %xmm0, %xmm0
addss   %xmm2, %xmm1
addss   %xmm0, %xmm1
```

The reduction cost partially offsets the SIMD multiply gain — this is why ijk is slower than ikj despite both being vectorized.

---

### `Gemm_tiled` — tiled kernel

Same vectorization pattern as ikj — broadcast + packed multiply-add on the inner `j` loop:

```asm
movss   (%rcx), %xmm2
movaps  %xmm2, %xmm3
shufps  $0, %xmm3, %xmm3   ; broadcast ak

.L88:
movups  (%r9,%rdx), %xmm0   ; load 4 B values
movups  (%rax,%rdx), %xmm4  ; load 4 C values
mulps   %xmm1, %xmm0        ; multiply 4 floats
addps   %xmm4, %xmm0        ; add 4 floats
movups  %xmm0, (%rax,%rdx)  ; store 4 floats
```

---

## What Was Not Found

```bash
grep -i ymm GEMM.s   # → nothing
grep -i vfmadd GEMM.s # → nothing
```

- No AVX instructions (`ymm` registers, 256-bit).
- No FMA instructions (`vfmadd`).

GCC stayed conservative under the default build — targeting a generic x86-64 baseline rather than the host CPU's actual capabilities.

---

## Instruction Glossary

| Instruction | Meaning |
|-------------|---------|
| `mulss`     | Scalar single-precision float multiply |
| `addss`     | Scalar single-precision float add |
| `mulps`     | Packed float multiply (4 floats, SSE) |
| `addps`     | Packed float add (4 floats, SSE) |
| `shufps`    | Shuffle packed floats (used here to broadcast) |
| `movups`    | Move unaligned packed floats |

---

## Conclusions

- GCC auto-vectorized all three GEMM kernels at `-O3` without any manual intrinsics.
- All vectorization used SSE (`xmm`, 128-bit) — 4 floats per instruction.
- No AVX, no FMA.
- The performance advantage of ikj over ijk is not explained by vectorization quality alone — both are vectorized. The difference comes from ikj avoiding a horizontal reduction, which is expensive relative to a straight packed store.
- The compiler is doing real work here, but it is being conservative about instruction set selection. See **SIMD Investigation 2** for what happens when that constraint is lifted.


# SIMD Investigation 2 — Native Architecture (`-march=native`)

## Goal

Determine whether allowing GCC to target the host CPU's full instruction set enables stronger vectorization and meaningfully improves GEMM performance.

The hypothesis: GCC was being conservative under the generic `-O3` build. Giving it access to the machine's actual capabilities (AVX2, FMA) should produce significantly better code and faster runtimes.

---

## Method

Added `-march=native` to the CMake build configuration and performed a clean rebuild.

Generated assembly for comparison:

```bash
g++ -O3 -march=native -std=c++20 -S GEMM.cpp -o GEMM_native.s
```

Queried GCC's target capabilities:

```bash
g++ -march=native -Q --help=target | grep avx
```

Inspected assembly for key patterns:

```bash
grep -i ymm GEMM_native.s    # AVX 256-bit
grep -i vfmadd GEMM_native.s # FMA
grep -i vmul GEMM_native.s   # AVX multiply
```

---

## CPU Features Detected

GCC reported:

```
-mavx   [enabled]
-mavx2  [enabled]
```

The Ryzen 5 5500U supports AVX2 and FMA. GCC is now allowed to generate them.

---

## Assembly Changes

### Before (`-O3` only)

```asm
; SSE only — 128-bit XMM registers
shufps    ; emulated broadcast
mulps     ; packed multiply (4 floats)
addps     ; packed add (4 floats)
xmm registers
```

No FMA. No AVX.

---

### After (`-O3 -march=native`)

```asm
vbroadcastss  %xmm2, %ymm1          ; broadcast ak into 8 lanes
vmovups       (%rsi,%rcx), %ymm0    ; load 8 B values
vfmadd213ps   (%rax,%rcx), %ymm1, %ymm0  ; C = ak * B + C (8 floats, fused)
vmovups       %ymm0, (%rax,%rcx)    ; store 8 floats
```

Three changes at once:

| Change | Before | After |
|--------|--------|-------|
| Register width | 128-bit XMM | 256-bit YMM |
| Floats per instruction | 4 | 8 |
| Broadcast | `shufps` (shuffle workaround) | `vbroadcastss` (hardware) |
| Multiply + Add | `mulps` + `addps` (2 instructions) | `vfmadd213ps` (1 instruction) |

---

## What FMA Does

Previously `C += ak * B` required two instructions:

```asm
mulps   ; ak * B
addps   ; + C
```

With FMA:

```asm
vfmadd213ps   ; C = ak * B + C  (one instruction, one cycle)
```

This is one of the most important instructions in high-performance matrix multiplication. OpenBLAS and BLIS are built around it.

---

## Benchmark Impact

Despite the assembly changing dramatically, benchmark performance changed only marginally.

The ikj kernel at 1000×1000 was already at ~66ms under plain `-O3`. Under `-march=native` it remained in the same range.

---

## What This Means

The assembly got significantly better. The runtime did not follow proportionally.

This disproves the assumption:

```
Wider SIMD + FMA = proportionally faster runtime
```

If arithmetic throughput were the bottleneck, doubling the SIMD width (4 → 8 floats) plus fusing multiply-add should have roughly halved execution time. It did not.

The conclusion: **the GEMM kernels are no longer primarily bottlenecked by arithmetic throughput**. The floating-point work is cheap enough that other factors dominate — most likely memory access latency and cache behavior.

---

## Updated Understanding of ikj Performance

Before this investigation:

```
ikj is fast because:
  - good cache locality
  - compiler auto-vectorization (SSE)
```

After this investigation:

```
ikj is fast because:
  - good cache locality           ← still the dominant factor
  - compiler auto-vectorization   ← now AVX2 + FMA under -march=native
```

And yet runtime barely changes. Cache locality is doing the heavy lifting, not SIMD width.

---

## Key Lesson

Assembly quality and benchmark performance are related, but not always proportional.

A substantial improvement in generated machine code does not necessarily translate into a substantial improvement in runtime. **Performance must always be verified by measurement**, not inferred from instruction quality alone.

The next meaningful gains will likely come from:

- Better blocking / tiling strategy
- Data packing to improve spatial locality
- Microkernel design that explicitly targets register reuse

Rather than simply enabling wider SIMD instructions.
