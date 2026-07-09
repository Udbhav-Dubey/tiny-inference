#include "GEMM.h"
#include <cassert>
#include <algorithm>
#include <iostream>
#include <immintrin.h>
Tensor Gemm(Tensor&a,Tensor&b){
   const float* A=a.data();
   const float* B=b.data();
    const int a_row=a.grow();
    const int a_col=a.gcol();
    const int b_row=b.grow();
    const int b_col=b.gcol();

    assert(a_col==b_row&&"need to get a.col==b.row equal for matrix multiply");
            // i considered letting a and b be interswitched if b.col was equal to a.row instead of current situation but it is now a rigid api that wants in one direction only , i dont know whats better enginnering being rigid or working with any needs to decide 

    Tensor c(a_row,b_col);
    float* C=c.data();
    for (int i=0;i<a_row;i++){
         for (int k=0;k<a_col;k++){
             float ak=A[i*a_col+k];
            for (int j=0;j<b_col;j++){
                C[i*b_col+j]+=ak*B[k*b_col+j];
            }
         }
    }
    return c;
}
Tensor Gemm_ijk(Tensor&a,Tensor&b){
    const float* A=a.data();
    const float* B=b.data();
    const int a_row=a.grow();
    const int a_col=a.gcol();
    const int b_row=b.grow();
    const int b_col=b.gcol();
    assert(a_col==b_row&&"need to get a.col==b.row equal for matrix multiply");
    Tensor c(a_row,b_col);
    float*C=c.data();
        for (int i=0;i<a_row;i++){
            for (int j=0;j<b_col;j++){
                float sum=0;
                for (int k=0;k<a_col;k++){
                    sum+=A[i*a_col+k]*B[k*b_col+j];
                }
                C[i*b_col+j]=sum;
            }
        }
        return c;
    }
Tensor Gemm_tiled(Tensor&a,Tensor&b,int block_size){
    const float* A=a.data();
    const float* B=b.data();
    const int a_row=a.grow();
    const int b_row=b.grow();
    const int a_col=a.gcol();
    const int b_col=b.gcol();
    assert(a_col==b_row&&"need to get a.col==b.row equal for matrix multiply");
    Tensor c(a_row,b_col);
    float*C=c.data();
    for (int i_t=0;i_t<a_row;i_t+=block_size){
        for (int j_t=0;j_t<b_col;j_t+=block_size){
            // here take C[i,j];
            for (int k_t=0;k_t<a_col;k_t+=block_size){
               // here take A[i,k]and B[k,j];
               const int i_end=std::min(block_size+i_t,a_row);
               const int k_end=std::min(k_t+block_size,a_col);
               const int j_end=std::min(block_size+j_t,b_col);
               for (int i=i_t;i<i_end;i++){
                    for (int j=j_t;j<j_end;j++){
                        float acc0=0,acc1=0,acc2=0,acc3=0;
                        int k=k_t;
                        const float *a_cache=&A[i*a_col];
                for (;k<k_end-3;k+=4){
                        acc0+=a_cache[k]*B[k*b_col+j];
                        acc1+=a_cache[k+1]*B[(k+1)*b_col+j];
                        acc2+=a_cache[k+2]*B[(k+2)*b_col+j];
                        acc3+=a_cache[k+3]*B[(k+3)*b_col+j];
                        
                        }
                float acc=C[i*b_col+j]+acc0+acc1+acc2+acc3;
                for (;k<k_end;k++){
                acc+=A[i*a_col+k]*B[k*b_col+j];
                }
                C[i*b_col+j]=acc;
                }
               }
            }
        }
    }
    return c;
}
Tensor Gemm_simd(Tensor&a,Tensor&b){
    const float*A=a.data();
    const float*B=b.data();
    const int a_row=a.grow(); 
    const int b_row=b.grow();
    const int a_col=a.gcol();
    const int b_col=b.gcol();
    assert(a_col==b_row&&"need to get a.col==b.row equal for matrix multiply");
    Tensor c(a_row,b_col);
    float*C=c.data();
    int jend=b_col-(b_col%8);
    for (int i=0;i<a_row;i++){
            for (int k=0;k<a_col;k++){
               __m256 as=_mm256_broadcast_ss(&A[i*a_col+k]);
               int j=0;
                for (;j<jend;j+=8){
                __m256 cs=_mm256_loadu_ps(&C[i*b_col+j]);
                __m256 bs=_mm256_loadu_ps(&B[k*b_col+j]);
                cs=_mm256_fmadd_ps(as,bs,cs);
            _mm256_storeu_ps(&C[i*b_col+j],cs);
            }
                for (;j<b_col;j++){
                    C[i*b_col+j]+=A[i*a_col+k]*B[k*b_col+j];
                }
        }
    } 
    return c;
}
Tensor Gemm_tiled_simd(Tensor&a,Tensor&b,int block_size){
    const float* A=a.data();
    const float* B=b.data();
    const int a_row=a.grow();
    const int b_row=b.grow();
    const int a_col=a.gcol();
    const int b_col=b.gcol();
    assert(a_col==b_row&&"need to get a.col==b.row equal for matrix multiply");
    Tensor c(a_row,b_col);
    float*C=c.data();
    for (int i_t=0;i_t<a_row;i_t+=block_size){
        for (int j_t=0;j_t<b_col;j_t+=block_size){
            for (int k_t=0;k_t<a_col;k_t+=block_size){
               const int i_end=std::min(block_size+i_t,a_row);
               const int k_end=std::min(k_t+block_size,a_col);
               const int j_end=std::min(block_size+j_t,b_col);
               for (int i=i_t;i<i_end;i++){
                    int j=j_t;
                    int j_simd_end=j_end-((j_end-j_t)%8);
                            const float *a_cache=&A[i*a_col];
                for (;j<j_simd_end;j+=8){
                             __m256 cs0=_mm256_setzero_ps();
                             __m256 cs1=_mm256_setzero_ps();
                             __m256 cs2=_mm256_setzero_ps();
                             __m256 cs3=_mm256_setzero_ps();
                                 int k=k_t;
                     for (;k<k_end-3;k+=4){
           //                 float ak=A[i*a_col+k];
             //               __m256 as=_mm256_set1_ps(ak);
                            __m256 as0=_mm256_broadcast_ss(&a_cache[k]);
                            __m256 as1=_mm256_broadcast_ss(&a_cache[k+1]);  
                            __m256 as2=_mm256_broadcast_ss(&a_cache[k+2]);
                            __m256 as3=_mm256_broadcast_ss(&a_cache[k+3]);
                            __m256 bs0=_mm256_loadu_ps(&B[k*b_col+j]);
                            __m256 bs1=_mm256_loadu_ps(&B[(k+1)*b_col+j]);
                            __m256 bs2=_mm256_loadu_ps(&B[(k+2)*b_col+j]);
                             __m256 bs3=_mm256_loadu_ps(&B[(k+3)*b_col+j]);
                            cs0=_mm256_fmadd_ps(as0,bs0,cs0);
                            cs1=_mm256_fmadd_ps(as1,bs1,cs1);
                            cs2=_mm256_fmadd_ps(as2,bs2,cs2);
                            cs3=_mm256_fmadd_ps(as3,bs3,cs3);
                        }
                        cs0=_mm256_add_ps(cs0,cs1);
                        cs2=_mm256_add_ps(cs2,cs3);
                        cs0=_mm256_add_ps(cs0,cs2);
                        for (;k<k_end;k++){
                            __m256 as=_mm256_broadcast_ss(&a_cache[k]);
                            __m256 bs=_mm256_loadu_ps(&B[k*b_col+j]);
                            cs0=_mm256_fmadd_ps(as,bs,cs0);
                        }
                        __m256 c_=_mm256_loadu_ps(&C[i*b_col+j]);
                        cs0=_mm256_add_ps(cs0,c_);
                        _mm256_storeu_ps(&C[i*b_col+j],cs0);
                }
                        for (;j<j_end;j++){
                            float acc=C[i*b_col+j];
                            for (int k=k_t;k<k_end;k++){
                        acc+=A[i*a_col+k]*B[k*b_col+j];
                            }
                            C[i*b_col+j]=acc;
                        }
               }
            }
        }
    }
    return c;
}

