#include "GEMM.h"
#include <cassert>
#include <algorithm>
#include <iostream>
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
                for (int k=k_t;k<k_end;k++){
                        float ak=A[i*a_col+k];
                    for (int j=j_t;j<j_end;j++){
                        C[i*b_col+j]+=ak*B[k*b_col+j];
                    }
                }
               }
            }
        }
    }
    return c;
}
