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
 
All runs in Release mode.
 
Here are your updated README tables based on the new benchmark data you provided.

The values have been formatted to match your previous style (`Time (ns)` using commas for readability, `Time (s)` rounded to 3 decimal places, and the `vs ikj` relative performance calculation using the total time values).

A new section for **2000 × 2000** has also been added since your new data includes it.

```


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



```
 
### Observations

* For matrix sizes up to 1000×1000, the flat `ikj` kernel remained competitive with or faster than most tiled variants. At these sizes, tile management overhead offsets much of the potential cache benefit.
* Increasing tile size from 16 → 32 → 64 → 128 → 256 generally improved performance, indicating that larger tiles reduce boundary overhead and improve data reuse.
* At 2000×2000, cache blocking becomes clearly beneficial. The tiled kernels outperform the flat `ikj` implementation, demonstrating that cache behavior becomes a dominant factor once the working set grows sufficiently large.
* `tiled(256)` produced the best overall result, achieving approximately **1.8× higher performance** than the flat `ikj` kernel at 2000×2000.
* Increasing the tile size further to 512 caused a significant regression, suggesting that the tile no longer fits efficiently within the processor's cache hierarchy and begins to lose the locality benefits that blocking is intended to provide.
* Earlier SIMD investigations confirmed that GCC already generates AVX2 and FMA instructions under `-march=native`, so the performance gains observed here are primarily attributable to improved cache utilization rather than improved vectorization.
* This experiment demonstrates that effective GEMM optimization is not solely about arithmetic throughput; once SIMD is largely solved by the compiler, cache locality and data movement become the primary factors determining performance can you give me this .md format please
 



