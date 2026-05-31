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
 
