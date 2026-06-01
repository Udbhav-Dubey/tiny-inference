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
 

