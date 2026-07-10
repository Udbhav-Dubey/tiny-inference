#include <iostream>
#include "../src/GEMM.h"
#include "../src/tensor.h"
#include <vector>
#include <cassert>
#include <immintrin.h>

using namespace std;
Tensor Gemm_tiled_simd_test(Tensor&a,Tensor&b,int block_size){
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
                        for (;j<j_simd_end;j+=8){
                            __m256 cs=_mm256_loadu_ps(&C[i*b_col+j]);
                for (int k=k_t;k<k_end;k++){
                            float ak=A[i*a_col+k];
                            __m256 as=_mm256_set1_ps(ak);
 //                           __m256 cs=_mm256_loadu_ps(&C[i*b_col+j]);
//                            cout << "cs : " << i*b_col + j << "and j_t -> " << i*b_col+j_t <<"\n"; 
                            __m256 bs=_mm256_loadu_ps(&B[k*b_col+j]);
                            cs=_mm256_fmadd_ps(as,bs,cs);
//                            _mm256_storeu_ps(&C[i*b_col+j],cs);
                    }
                           _mm256_storeu_ps(&C[i*b_col+j],cs);
  
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
    }
    return c;
}
void printit(Tensor&x){
    int x_col=x.gcol();
    int x_row=x.grow();
    float*X=x.data();
    for (int i=0;i<x_col*x_row;i++){
        if (i!=0&&i%x_col==0)std::cout << "\n";
        std::cout << X[i] << " ";
    }
    std::cout << "\n";
}
void printit(float*x){
    for (int i=0;i<4;i++){
        if (i%4==0){std::cout << "\n";}
        std::cout << x[i] << " ";
    }
    std::cout << " \n";
}

int main (){
     Tensor A(8,24);
    for (int i=0;i<8;i++){
       for (int j=0;j<24;j++){
        A.set_val(i,j,i+j);
       } 
    }
    Tensor B(24,16);
     for (int i=0;i<24;i++){
       for (int j=0;j<16;j++){
        B.set_val(i,j,(i+j)%7);
       } 
    }
     Tensor C2=Gemm_tiled_simd_test(A,B,8);
    std::cout << "c2:\n";
    printit(C2);
   
    return 0;
}
