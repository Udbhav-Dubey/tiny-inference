#pragma once
#include "tensor.h"
Tensor Gemm(Tensor&,Tensor&);
Tensor Gemm_ijk(Tensor&,Tensor&);
Tensor Gemm_tiled(Tensor&,Tensor&,int block_size);
Tensor Gemm_simd(Tensor&,Tensor&);
