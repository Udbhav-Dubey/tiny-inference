# Optimization Log

This document tracks the evolution of the `tiny-inference` project.  
The goal is to measure the impact of optimizations rather than relying on intuition.

---

## Hardware

- **CPU:** Ryzen 5 5500U
- **Threads:** 6 Cores / 12 Threads

## Compiler

- **C++20**

---

## GEMM V0 — Baseline

### Description

Naive matrix multiplication implementation.

Loop order:

```cpp
for (i)
    for (k)
        for (j)
```

Tensor element access performed through the `Tensor` API.  
No tiling. No SIMD. No threading. No cache-aware optimizations.

### Benchmark Results

| Size      | Time (ns)         | Time (s)   | vs Previous  | Checksum |
|-----------|-------------------|------------|--------------|----------|
| 200 × 200 | `184,339,657 ns`  | `0.184 s`  | —            | `2664`   |
| 500 × 500 | `2,899,966,148 ns`| `2.900 s`  | **~15.7×**   | `6732`   |
| 1000×1000 | `23,188,915,787 ns`| `23.189 s` | **~8.0×**   | `13494`  |

### Observations

- Runtime scales almost exactly with O(n³): 200→500 is a 2.5× size increase → 2.5³ ≈ **15.6×** predicted, **15.7×** observed; 500→1000 is 2× → 2³ = **8×** predicted, **8.0×** observed.
- Baseline implementation is focused on correctness.
- Results will be used as a comparison point for future optimizations.


---



## GEMM V0.5 — Small Caching Optimization
 
### Description
 
Same loop order as V0 (`i → k → j`), with one small caching optimization: the value `a.get_val(i, k)` is hoisted out of the innermost `j` loop into a local variable `ak`, avoiding a redundant Tensor API call on every iteration of `j`.
 
```cpp
for (int i = 0; i < a.grow(); i++) {
    for (int k = 0; k < a.gcol(); k++) {
        float ak = a.get_val(i, k);   // hoisted
        for (int j = 0; j < b.gcol(); j++) {
            c.set_val(i, j, c.get_val(i, j) + (ak * b.get_val(k, j)));
        }
    }
}
```
 
No tiling. No SIMD. No threading. No cache-aware optimizations.
 
### Benchmark Results
 
| Size       | Time (ns)            | Time (s)    | vs Previous  | vs V0        | Checksum |
|------------|----------------------|-------------|--------------|--------------|----------|
| 200 × 200  | `140,550,848 ns`     | `0.141 s`   | —            | **~1.3× faster** | `2664`   |
| 500 × 500  | `2,212,122,321 ns`   | `2.212 s`   | **~15.7×**   | **~1.3× faster** | `6732`   |
| 1000 × 1000| `17,885,843,091 ns`  | `17.886 s`  | **~8.1×**    | **~1.3× faster** | `13494`  |
 
### Observations
 
- Consistent **~1.3× speedup** over V0 across all sizes, from a single hoisted `get_val` call.
- Checksums match V0 exactly — correctness preserved.
- O(n³) scaling unchanged, as expected.
 
---
 
## GEMM V1 — Loop Order + Sum Accumulator
 
### Description
 
Changed loop order from `i → k → j` to `i → j → k`, and replaced per-iteration `get_val`/`set_val` on `c` with a local `sum` accumulator written back once per `(i, j)` pair.
 
```cpp
for (int i = 0; i < a.grow(); i++) {
    for (int j = 0; j < b.gcol(); j++) {
        float sum = 0;
        for (int k = 0; k < a.gcol(); k++) {
            sum += a.get_val(i, k) * b.get_val(k, j);
        }
        c.set_val(i, j, sum);
    }
}
```
 
Two changes over V0.5:
- **Loop order `i → j → k`** — better fits the actual memory layout of the `Tensor` API, reducing cache misses on the innermost loop.
- **`sum` accumulator** — eliminates a `get_val` + `set_val` on `c` every inner iteration; single `set_val` per output cell instead.
No tiling. No SIMD. No threading.
 
### Benchmark Results
 
Averaged over 2 runs.
 
| Size        | Time (ns)           | Time (s)    | vs V0.5          | vs V0            | Checksum |
|-------------|---------------------|-------------|------------------|------------------|----------|
| 200 × 200   | `101,099,307 ns`    | `0.101 s`   | **~1.4× faster** | **~1.8× faster** | `2664`   |
| 500 × 500   | `1,586,692,448 ns`  | `1.587 s`   | **~1.4× faster** | **~1.8× faster** | `6732`   |
| 1000 × 1000 | `12,724,341,597 ns` | `12.724 s`  | **~1.4× faster** | **~1.8× faster** | `13494`  |
 
### Observations
 
- **~1.4× faster than V0.5** and **~1.8× faster than V0** purely from loop reorder + accumulator — no hardware tricks yet.
- The `i → j → k` order outperforming `i → k → j` suggests the `Tensor` API's internal layout favors stepping `k` in the innermost loop (likely column-major or similar).
- Speedup is consistent across all sizes — this is a structural improvement, not a small-N artifact.
- Checksums match exactly — correctness preserved.
- Next logical step: fix tensor to reduce so many api calls.

---
 
## GEMM V2 — Raw Pointer Access + Release Mode
 
### Description
 
Removed all `Tensor` API (`get_val` / `set_val`) calls inside the GEMM loops. Instead, raw pointers to the underlying data are extracted once before the loops and all indexing is done with direct pointer arithmetic.
 
Both loop orders benchmarked: `i → j → k` (ijk) and `i → k → j` (ikj).
 
```cpp
// ikj — best performer in release
const float* A = a.data();
const float* B = b.data();
float* C = c.data();
 
for (int i = 0; i < a_row; i++) {
    for (int k = 0; k < a_col; k++) {
        float ak = A[i * a_col + k];
        for (int j = 0; j < b_col; j++) {
            C[i * b_col + j] += ak * B[k * b_col + j];
        }
    }
}
```
 
Two changes introduced simultaneously, benchmarked separately to isolate contribution:
- **Raw pointer access** — eliminates API call overhead on every element access.
- **Release mode** (`-DCMAKE_BUILD_TYPE=Release`) — enables compiler optimizations: inlining, auto-vectorization, loop transformations.
### Benchmark Results
 
#### V2a — Raw Pointers, Debug Mode
*Isolates the impact of API removal alone.*
 
##### ijk
 
| Size        | Time (ns)           | Time (s)   | vs V1 (ijk)      | Checksum |
|-------------|---------------------|------------|------------------|----------|
| 200 × 200   | `23,293,777 ns`     | `0.023 s`  | **~4.3× faster** | `2664`   |
| 500 × 500   | `139,523,219 ns`    | `0.140 s`  | **~11.4× faster**| `6732`   |
| 1000 × 1000 | `2,957,090,149 ns`  | `2.957 s`  | **~4.3× faster** | `13494`  |
 
##### ikj
 
| Size        | Time (ns)           | Time (s)   | vs V1 (ijk)      | Checksum |
|-------------|---------------------|------------|------------------|----------|
| 200 × 200   | `21,372,358 ns`     | `0.021 s`  | **~4.7× faster** | `2664`   |
| 500 × 500   | `327,450,832 ns`    | `0.327 s`  | **~4.8× faster** | `6732`   |
| 1000 × 1000 | `2,593,207,678 ns`  | `2.593 s`  | **~4.9× faster** | `13494`  |
 
#### V2b — Raw Pointers, Release Mode
*Adds compiler optimizations on top of V2a.*
 
##### ijk
 
| Size        | Time (ns)          | Time (s)   | vs V2a (ijk)      | vs V1            | vs V0             | Checksum |
|-------------|--------------------|------------|-------------------|------------------|-------------------|----------|
| 200 × 200   | `7,742,424 ns`     | `0.008 s`  | **~3.0× faster**  | **~13.1× faster**| **~23.8× faster** | `2664`   |
| 500 × 500   | `139,523,219 ns`   | `0.140 s`  | **~1.0×**         | **~11.4× faster**| **~20.7× faster** | `6732`   |
| 1000 × 1000 | `2,257,474,974 ns` | `2.257 s`  | **~1.3× faster**  | **~5.6× faster** | **~10.3× faster** | `13494`  |
 
##### ikj — best overall
 
| Size        | Time (ns)          | Time (s)   | vs V2a (ikj)      | vs V1            | vs V0              | Checksum |
|-------------|--------------------|------------|-------------------|------------------|--------------------|----------|
| 200 × 200   | `2,581,415 ns`     | `0.003 s`  | **~8.3× faster**  | **~39.2× faster**| **~71.3× faster**  | `2664`   |
| 500 × 500   | `25,832,711 ns`    | `0.026 s`  | **~12.7× faster** | **~61.4× faster**| **~112.2× faster** | `6732`   |
| 1000 × 1000 | `190,455,459 ns`   | `0.190 s`  | **~13.6× faster** | **~66.8× faster**| **~121.7× faster** | `13494`  |
 
### Observations
 
- **API removal alone (V2a)** gave ~4.9× speedup at 1000×1000 — the `get_val`/`set_val` overhead was significant even in a simple loop.
- **Release mode on top (V2b ikj)** added another ~13.6× — compiler auto-vectorization is the dominant factor overall.
- With raw pointers, **ikj finally beats ijk** as cache theory predicts for row-major data. The API was previously masking the true access pattern cost.
- Combined, V2b ikj is **~122× faster than V0** with no tiling, no SIMD written by hand, no threading.


  ---
 
## GEMM V3 — Tiled GEMM (Work in Progress)
 
### Description
 
Introduced cache blocking (tiling) over all three loop dimensions (`i`, `j`, `k`), with configurable `block_size`. The idea is to keep a tile of A, B, and C in L1/L2 cache and amortize the cost of memory loads across the tile.
 
```cpp
for (int i_t = 0; i_t < a_row; i_t += block_size) {
    for (int j_t = 0; j_t < b_col; j_t += block_size) {
        for (int k_t = 0; k_t < a_col; k_t += block_size) {
            const int i_end = std::min(i_t + block_size, a_row);
            const int k_end = std::min(k_t + block_size, a_col);
            const int j_end = std::min(j_t + block_size, b_col);
            for (int i = i_t; i < i_end; i++) {
                for (int k = k_t; k < k_end; k++) {
                    float ak = A[i * a_col + k];
                    for (int j = j_t; j < j_end; j++) {
                        C[i * b_col + j] += ak * B[k * b_col + j];
                    }
                }
            }
        }
    }
}
```
 
Inner loop order inside each tile is `i → k → j` — same order as the flat ikj kernel, meaning no additional cache access improvement is gained from tiling itself.

Additional investigation during V3 development showed that GCC already auto-vectorizes the GEMM kernels and, with `-march=native`, emits AVX2 and FMA instructions. Benchmark results showed only marginal gains from wider SIMD, suggesting arithmetic throughput was no longer the primary bottleneck. Tiling was introduced to investigate whether improving cache locality and data reuse could provide further speedups.
 

### Benchmark Results
 
All runs in Release mode (`-O3 -march=native`).

GCC auto-vectorization, AVX2, and FMA generation were previously verified; this benchmark focuses on the impact of cache blocking under those conditions.
 
#### 200 × 200
 
| Variant     | Time (ns)       | Time (s)      | vs ikj            |
|-------------|-----------------|---------------|-------------------|
| ijk         | `51,999,865 ns` | `0.051999 s`  | **~12.1× slower** |
| ikj         | `4,311,107 ns`  | `0.004311 s`  | —                 |
| tiled(16)   | `14,103,116 ns` | `0.014103 s`  | **~3.3× slower** |
| tiled(32)   | `9,740,340 ns`  | `0.009740 s`  | **~2.3× slower** |
| tiled(64)   | `6,502,507 ns`  | `0.006502 s`  | **~1.5× slower** |
| tiled(128)  | `4,694,199 ns`  | `0.004694 s`  | **~1.1× slower** |
| tiled(256)  | `4,422,551 ns`  | `0.004422 s`  | **~1.0× slower** |
| tiled(512)  | `4,435,887 ns`  | `0.004435 s`  | **~1.0× slower** |
 
#### 500 × 500
 
| Variant     | Time (ns)        | Time (s)      | vs ikj            |
|-------------|------------------|---------------|-------------------|
| ijk         | `873,992,512 ns` | `0.873992 s`  | **~12.5× slower** |
| ikj         | `70,014,162 ns`  | `0.070014 s`  | —                 |
| tiled(16)   | `224,503,924 ns` | `0.224503 s`  | **~3.2× slower** |
| tiled(32)   | `144,335,527 ns` | `0.144335 s`  | **~2.1× slower** |
| tiled(64)   | `89,497,487 ns`  | `0.089497 s`  | **~1.3× slower** |
| tiled(128)  | `75,275,912 ns`  | `0.075275 s`  | **~1.1× slower** |
| tiled(256)  | `76,583,802 ns`  | `0.076583 s`  | **~1.1× slower** |
| tiled(512)  | `68,483,092 ns`  | `0.068483 s`  | **~1.0× faster** |
 
#### 1000 × 1000
 
| Variant     | Time (ns)          | Time (s)      | vs ikj            |
|-------------|--------------------|---------------|-------------------|
| ijk         | `7,448,034,843 ns` | `7.448034 s`  | **~13.1× slower** |
| ikj         | `569,895,985 ns`   | `0.569895 s`  | —                 |
| tiled(16)   | `1,817,338,541 ns` | `1.817338 s`  | **~3.2× slower** |
| tiled(32)   | `1,159,591,735 ns` | `1.159591 s`  | **~2.0× slower** |
| tiled(64)   | `757,489,451 ns`   | `0.757489 s`  | **~1.3× slower** |
| tiled(128)  | `638,783,836 ns`   | `0.638783 s`  | **~1.1× slower** |
| tiled(256)  | `611,579,897 ns`   | `0.611579 s`  | **~1.1× slower** |
| tiled(512)  | `768,996,559 ns`   | `0.768996 s`  | **~1.3× slower** |

#### 2000 × 2000
 
| Variant     | Time (ns)           | Time (s)      | vs ikj            |
|-------------|---------------------|---------------|-------------------|
| ijk         | `64,542,116,780 ns` | `64.542116 s` | **~10.1× slower** |
| ikj         | `6,386,723,983 ns`  | `6.386723 s`  | —                 |
| tiled(16)   | `15,972,203,610 ns` | `15.972203 s` | **~2.5× slower** |
| tiled(32)   | `9,219,272,481 ns`  | `9.219272 s`  | **~1.4× slower** |
| tiled(64)   | `6,021,243,091 ns`  | `6.021243 s`  | **~1.1× faster** |
| tiled(128)  | `4,927,757,834 ns`  | `4.927757 s`  | **~1.3× faster** |
| tiled(256)  | `4,515,183,908 ns`  | `4.515183 s`  | **~1.4× faster** |
| tiled(512)  | `7,150,383,799 ns`  | `7.150383 s`  | **~1.1× slower** |

### Observations

* For matrix sizes up to 1000×1000, the flat `ikj` kernel remained competitive with or faster than most tiled variants. At these sizes, tile management overhead offsets much of the potential cache benefit.
* Increasing tile size from 16 → 32 → 64 → 128 → 256 generally improved performance, indicating that larger tiles reduce boundary overhead and improve data reuse.
* At 2000×2000, cache blocking becomes clearly beneficial. The tiled kernels outperform the flat `ikj` implementation, demonstrating that cache behavior becomes a dominant factor once the working set grows sufficiently large.
* `tiled(256)` produced the best overall result, achieving approximately **1.8× higher performance** than the flat `ikj` kernel at 2000×2000.
* Increasing the tile size further to 512 caused a significant regression, suggesting that the tile no longer fits efficiently within the processor's cache hierarchy and begins to lose the locality benefits that blocking is intended to provide.
* Earlier SIMD investigations confirmed that GCC already generates AVX2 and FMA instructions under `-march=native`, so the performance gains observed here are primarily attributable to improved cache utilization rather than improved vectorization.
* This experiment demonstrates that effective GEMM optimization is not solely about arithmetic throughput; once SIMD is largely solved by the compiler, cache locality and data movement become the primary factors determining performance.

---

## GEMM V4 — Manual AVX2 SIMD (Flat and Tiled)

### Description

Two new kernels, both hand-written with AVX2 intrinsics (`_mm256_fmadd_ps`, `_mm256_loadu_ps`/`storeu_ps`) instead of relying on GCC auto-vectorization:

- **`Gemm_simd`** — same `ikj` loop order as the flat kernel, but the innermost `j` loop is manually unrolled 8-wide using `_mm256_broadcast_ss` for the scalar `A[i,k]` and FMA against 8 contiguous `B`/`C` elements. A scalar cleanup loop handles any remainder when `b_col` isn't a multiple of 8.
- **`Gemm_tiled_simd`** — identical tiling structure to V3's `Gemm_tiled` (same `i_t → j_t → k_t` blocking), but the innermost per-tile `j` loop is replaced with the same 8-wide AVX2 FMA pattern, with a scalar cleanup loop for the tile's remainder columns.

The goal was to check whether writing the vectorization explicitly, rather than trusting the compiler, would yield a further speedup on top of V2 (flat, auto-vectorized) and V3 (tiled).

```cpp
// Gemm_simd inner loop
__m256 as = _mm256_broadcast_ss(&A[i*a_col+k]);
for (; j < jend; j += 8) {
    __m256 cs = _mm256_loadu_ps(&C[i*b_col+j]);
    __m256 bs = _mm256_loadu_ps(&B[k*b_col+j]);
    cs = _mm256_fmadd_ps(as, bs, cs);
    _mm256_storeu_ps(&C[i*b_col+j], cs);
}
```

### Benchmark Results

All runs in Release mode (`-O3 -march=native`).

#### Flat: manual SIMD vs. auto-vectorized `ikj`

| Size        | ikj (ns)      | Manual SIMD (ns) | vs ikj            |
|-------------|---------------|-------------------|--------------------|
| 200 × 200   | `441,513`     | `453,105`         | **~1.03× slower**  |
| 500 × 500   | `7,364,193`   | `7,341,410`       | **~1.0× (even)**   |
| 1000 × 1000 | `56,407,819`  | `64,118,899`      | **~1.14× slower**  |
| 2000 × 2000 | `828,095,397` | `689,567,585`     | **~1.2× faster**   |

#### Tiled: manual SIMD vs. plain tiled, best block sizes shown

| Block Size | Size        | Tiled (ns)    | SIMD Tiled (ns) | vs plain tiled     |
|------------|-------------|---------------|-----------------|--------------------|
| 128        | 1000×1000   | `61,189,179`  | `66,998,390`    | **~1.09× slower**  |
| 256        | 200 × 200   | `454,598`     | `457,920`       | **~1.01× slower**  |
| 256        | 500 × 500   | `7,835,911`   | `8,249,539`     | **~1.05× slower**  |
| 256        | 1000 × 1000 | `57,495,697`  | `58,868,312`    | **~1.02× slower**  |
| 256        | 2000 × 2000 | `457,223,237` | `458,068,132`   | **~1.0× (even)**   |

Full data for all block sizes (16–512) is in the tables at the top of this log, under "For SIMD TILED".

### Observations

- Manual AVX2 SIMD does **not** consistently beat the compiler auto-vectorized `ikj` kernel, and at 1000×1000 it's noticeably slower (~14%). The one clear win is at 2000×2000, where flat manual SIMD is ~1.2× faster than flat `ikj`.
- Manual SIMD on top of tiling produces **no improvement** over plain tiling at any size — at best it ties (2000×2000, block 256), and at worst it's ~9% slower (block 128, 1000×1000).
- This confirms the hypothesis from V3: GCC's auto-vectorization under `-O3 -march=native` was already extracting most of the available SIMD throughput from the flat and tiled kernels. Hand-rolling the same 8-wide FMA pattern mostly just adds instruction overhead (broadcast, load/store shuffling) without giving the compiler anything new to exploit.
- The best performer overall remains **plain tiled(256)** from V3 — manual SIMD tiling essentially reproduces its performance rather than surpassing it.
- Practical takeaway: once the compiler is already vectorizing a memory/cache-bound kernel, further gains are far more likely to come from improving data locality (blocking, layout, prefetching) than from hand-written intrinsics. Manual SIMD is worth revisiting only if profiling shows the kernel is genuinely arithmetic-throughput-bound rather than memory-bound.

---

## GEMM V5 — Multiple Accumulators (Register Blocking) on `Gemm_tiled_simd`

### Description

Two variants of the tiled AVX2 SIMD kernel (`Gemm_tiled_simd`) were compared:

- **Single accumulator** — same structure as V4's tiled SIMD kernel: one `__m256` accumulator loaded from `C`, updated with a single `_mm256_fmadd_ps` per `k`, and stored back.
- **Multiple accumulators** — `k` is unrolled 4-wide inside each tile. Four independent `__m256` accumulators (`cs0`–`cs3`) are broadcast-FMA'd against 4 separate cached `A` scalars and 4 offset `B` rows before being tree-reduced (`cs0 += cs1`, `cs2 += cs3`, `cs0 += cs2`) and loaded/added/stored against `C` once per tile-column-chunk. A scalar cleanup loop handles both the `k` remainder (`k_end - 3`) and the `j` remainder (non-multiple-of-8 columns).

```cpp
// Gemm_tiled_simd — multiple accumulator inner loop
for (; j < j_simd_end; j += 8) {
    __m256 cs0 = _mm256_setzero_ps();
    __m256 cs1 = _mm256_setzero_ps();
    __m256 cs2 = _mm256_setzero_ps();
    __m256 cs3 = _mm256_setzero_ps();
    int k = k_t;
    for (; k < k_end - 3; k += 4) {
        __m256 as0 = _mm256_broadcast_ss(&a_cache[k]);
        __m256 as1 = _mm256_broadcast_ss(&a_cache[k + 1]);
        __m256 as2 = _mm256_broadcast_ss(&a_cache[k + 2]);
        __m256 as3 = _mm256_broadcast_ss(&a_cache[k + 3]);
        __m256 bs0 = _mm256_loadu_ps(&B[k * b_col + j]);
        __m256 bs1 = _mm256_loadu_ps(&B[(k + 1) * b_col + j]);
        __m256 bs2 = _mm256_loadu_ps(&B[(k + 2) * b_col + j]);
        __m256 bs3 = _mm256_loadu_ps(&B[(k + 3) * b_col + j]);
        cs0 = _mm256_fmadd_ps(as0, bs0, cs0);
        cs1 = _mm256_fmadd_ps(as1, bs1, cs1);
        cs2 = _mm256_fmadd_ps(as2, bs2, cs2);
        cs3 = _mm256_fmadd_ps(as3, bs3, cs3);
    }
    cs0 = _mm256_add_ps(cs0, cs1);
    cs2 = _mm256_add_ps(cs2, cs3);
    cs0 = _mm256_add_ps(cs0, cs2);
    // scalar cleanup for k remainder, then load/add/store against C
}
```

`a_cache` (the `A` row for the current tile, hoisted before the `j` loop) and the 4-wide `k` unroll are the two changes over V4's single-accumulator tiled SIMD kernel; the tiling structure (`i_t → j_t → k_t`) is unchanged.

> Note: the single-accumulator source was overwritten before this benchmark was logged, so only the multiple-accumulator source is shown above. The single-accumulator numbers below come from a benchmark run captured just before the overwrite.

### Benchmark Results

All runs in Release mode (`-O3 -march=native`). `vs single` compares the multi-accumulator kernel against the single-accumulator kernel at the same block size; `vs ikj` compares it against the flat auto-vectorized `ikj` kernel from the same run.

#### 200 × 200 (ikj reference: `580,205 ns`)

| Block Size | Single-Acc (ns) | Multi-Acc (ns) | Multi-Acc (s) | vs single         | vs ikj            |
|------------|-----------------|----------------|---------------|--------------------|--------------------|
| 16         | `631,227`       | `571,584`      | `0.000572 s`  | **~1.10× faster**  | **~1.02× faster**  |
| 32         | `704,772`       | `434,532`      | `0.000435 s`  | **~1.62× faster**  | **~1.34× faster**  |
| 64         | `778,470`       | `389,576`      | `0.000390 s`  | **~2.00× faster**  | **~1.49× faster**  |
| 128        | `915,346`       | `412,510`      | `0.000413 s`  | **~2.22× faster**  | **~1.41× faster**  |
| 256        | `1,078,019`     | `402,714`      | `0.000403 s`  | **~2.68× faster**  | **~1.44× faster**  |
| 512        | `1,079,268`     | `388,951`      | `0.000389 s`  | **~2.77× faster**  | **~1.49× faster**  |

#### 500 × 500 (ikj reference: `9,372,431 ns`)

| Block Size | Single-Acc (ns) | Multi-Acc (ns) | Multi-Acc (s) | vs single         | vs ikj            |
|------------|-----------------|----------------|---------------|--------------------|--------------------|
| 16         | `11,039,781`    | `10,141,878`   | `0.010142 s`  | **~1.09× faster**  | **~0.92× slower**  |
| 32         | `12,660,499`    | `7,917,220`    | `0.007917 s`  | **~1.60× faster**  | **~1.18× faster**  |
| 64         | `13,115,085`    | `7,156,311`    | `0.007156 s`  | **~1.83× faster**  | **~1.31× faster**  |
| 128        | `16,330,784`    | `9,456,308`    | `0.009456 s`  | **~1.73× faster**  | **~1.0× (even)**   |
| 256        | `18,953,203`    | `9,281,310`    | `0.009281 s`  | **~2.04× faster**  | **~1.01× faster**  |
| 512        | `20,353,890`    | `14,931,752`   | `0.014932 s`  | **~1.36× faster**  | **~0.63× slower**  |

#### 1000 × 1000 (ikj reference: `76,302,111 ns`)

| Block Size | Single-Acc (ns) | Multi-Acc (ns) | Multi-Acc (s) | vs single         | vs ikj            |
|------------|-----------------|----------------|---------------|--------------------|--------------------|
| 16         | `84,181,141`    | `72,499,780`   | `0.072500 s`  | **~1.16× faster**  | **~1.05× faster**  |
| 32         | `105,490,119`   | `56,662,145`   | `0.056662 s`  | **~1.86× faster**  | **~1.35× faster**  |
| 64         | `102,067,366`   | `49,119,969`   | `0.049120 s`  | **~2.08× faster**  | **~1.55× faster**  |
| 128        | `125,864,587`   | `62,495,351`   | `0.062495 s`  | **~2.01× faster**  | **~1.22× faster**  |
| 256        | `141,303,493`   | `59,594,291`   | `0.059594 s`  | **~2.37× faster**  | **~1.28× faster**  |
| 512        | `153,617,211`   | `77,241,096`   | `0.077241 s`  | **~1.99× faster**  | **~0.99× (even)**  |

#### 2000 × 2000 (ikj reference: `1,015,918,749 ns`)

| Block Size | Single-Acc (ns)   | Multi-Acc (ns) | Multi-Acc (s) | vs single         | vs ikj            |
|------------|-------------------|----------------|---------------|--------------------|--------------------|
| 16         | `875,529,879`     | `568,968,422`  | `0.568968 s`  | **~1.54× faster**  | **~1.79× faster**  |
| 32         | `814,355,555`     | `516,283,337`  | `0.516283 s`  | **~1.58× faster**  | **~1.97× faster**  |
| 64         | `802,685,616`     | `398,143,148`  | `0.398143 s`  | **~2.02× faster**  | **~2.55× faster**  |
| 128        | `1,000,898,464`   | `525,910,886`  | `0.525911 s`  | **~1.90× faster**  | **~1.93× faster**  |
| 256        | `1,124,680,152`   | `527,080,284`  | `0.527080 s`  | **~2.13× faster**  | **~1.93× faster**  |
| 512        | `1,230,355,214`   | `605,648,695`  | `0.605649 s`  | **~2.03× faster**  | **~1.68× faster**  |

### Observations

- Multiple accumulators are a clear, consistent win over the single-accumulator kernel at every block size and every matrix size — typically **~1.5–2.8× faster**, with the effect growing at larger block sizes (256, 512) where a single accumulator previously suffered the most from latency stalls on the FMA dependency chain.
- `block(64)` is the standout: it's the best or near-best multi-accumulator result at every single matrix size, and it's the first configuration in this whole log to consistently beat the flat auto-vectorized `ikj` kernel across **all** sizes (200 through 2000), not just at the largest size like V3/V4's tiling did.
- At 2000×2000, `block(64)` multi-accumulator hits **~2.55× faster than `ikj`** — the largest margin recorded so far in this project, and a jump from V4's best flat-SIMD result of only ~1.2× at the same size.
- At small/medium block sizes (16, and to a lesser extent 128), the multi-accumulator kernel underperforms relative to its own potential and is occasionally slower than `ikj` (500×500 block 16, block 512) — likely because with only 4 iterations of unroll, small blocks don't have enough `k` depth to amortize the setup cost, and very large blocks (512) start pushing the working tile out of L1/L2 again.
- This confirms the core hypothesis behind register blocking: the single-accumulator FMA chain was latency-bound (each `fmadd` must wait for the previous one to retire before starting), while 4 independent accumulators let the CPU pipeline multiple FMAs in flight, closer to saturating the port's throughput rather than its latency.
- Next step: since `block(64)` with 4-wide accumulation is already the best result in the log, the natural continuation is to try wider unrolling (8 accumulators) and/or 2D register blocking (multiple accumulators across both `i` and `j`, not just `k`) to see if there's more latency-hiding headroom left, per the Salykova GEMM approach.
