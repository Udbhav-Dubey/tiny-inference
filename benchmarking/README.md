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
