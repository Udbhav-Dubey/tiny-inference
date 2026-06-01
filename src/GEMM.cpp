#include "GEMM.h"
#include <cassert>
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

