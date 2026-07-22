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
    assert(block_size>=2&&block_size%2==0&&"block_size must be an even number and >= 2");
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
    assert(block_size>=2&&block_size%2==0&&"block_size must be an even number and >= 2");
    const float* A=a.data();
    const float* B=b.data();
    const int a_row=a.grow();
    const int b_row=b.grow();
    const int a_col=a.gcol();
    const int b_col=b.gcol();
    assert(a_col==b_row&&"need to get a.col==b.row equal for matrix multiply");
    Tensor c(a_row,b_col);
    float*C=c.data();
     Tensor b_pack(block_size,block_size);
    float*B_pack=b_pack.data();
    for (int k_t=0;k_t<a_col;k_t+=block_size){
            for (int j_t=0;j_t<b_col;j_t+=block_size){
                for (int k1=0;k1<block_size;k1++){
                    for (int j1=0;j1<block_size;j1++){
                        B_pack[k1*block_size+j1]=B[(k_t+k1)*b_col+(j_t+j1)];
                        }
                    }

               for (int i_t=0;i_t<a_row;i_t+=block_size){
                const int i_end=std::min(block_size+i_t,a_row);
                const int k_end=std::min(k_t+block_size,a_col);
                const int j_end=std::min(block_size+j_t,b_col);
                int i=i_t;
                int i_simd_end = i_end - ((i_end - i_t) % 2);
                    for (;i<i_simd_end;i+=2){
                        int j=j_t;
                        int j_simd_end=j_end-((j_end-j_t)%8);
                       for (;j<j_simd_end;j+=8){
                     
                             __m256 c00=_mm256_setzero_ps();
                             __m256 c01=_mm256_setzero_ps();
                             __m256 c02=_mm256_setzero_ps();
                             __m256 c03=_mm256_setzero_ps();
                             __m256 c10=_mm256_setzero_ps();
                             __m256 c11=_mm256_setzero_ps();
                             __m256 c12=_mm256_setzero_ps();
                             __m256 c13=_mm256_setzero_ps();
                             //-> here you will handle k
                                 int k=k_t;
                     for (;k<k_end-3;k+=4){
           //                 float ak=A[i*a_col+k];
             //               __m256 as=_mm256_set1_ps(ak);
                        //    __m256 b0=_mm256_loadu_ps(&B[k*b_col+j]);
                            __m256 b0=_mm256_loadu_ps(&B_pack[(k-k_t)*block_size+(j-j_t)]);
                            __m256 a00=_mm256_set1_ps(A[i*a_col+k]);
                            __m256 a10=_mm256_set1_ps(A[(i+1)*a_col+k]);
                            c00=_mm256_fmadd_ps(a00,b0,c00);
                            c10=_mm256_fmadd_ps(a10,b0,c10);
                            //__m256 b1=_mm256_loadu_ps(&B[(k+1)*b_col+j]);
                            __m256 b1=_mm256_loadu_ps(&B_pack[((k-k_t)+1)*block_size+(j-j_t)]);
                            __m256 a01=_mm256_set1_ps(A[i*a_col+(k+1)]);
                            __m256 a11=_mm256_set1_ps(A[(i+1)*a_col+(k+1)]);
                            c01=_mm256_fmadd_ps(a01,b1,c01);
                            c11=_mm256_fmadd_ps(a11,b1,c11);
                            //__m256 b2=_mm256_loadu_ps(&B[(k+2)*b_col+j]);
                            __m256 b2=_mm256_loadu_ps(&B_pack[((k-k_t)+2)*block_size+(j-j_t)]);
                            __m256 a02=_mm256_set1_ps(A[i*a_col+(k+2)]);
                            __m256 a12=_mm256_set1_ps(A[(i+1)*a_col+(k+2)]);
                            c02=_mm256_fmadd_ps(a02,b2,c02);
                            c12=_mm256_fmadd_ps(a12,b2,c12);
                            //__m256 b3=_mm256_loadu_ps(&B[(k+3)*b_col+j]);
                            __m256 b3=_mm256_loadu_ps(&B_pack[((k-k_t)+3)*block_size+(j-j_t)]);
                            __m256 a03=_mm256_set1_ps(A[i*a_col+(k+3)]);
                            __m256 a13=_mm256_set1_ps(A[(i+1)*a_col+(k+3)]);
                            c03=_mm256_fmadd_ps(a03,b3,c03);
                            c13=_mm256_fmadd_ps(a13,b3,c13);
                        }
                        c00=_mm256_add_ps(c00,c01);
                        c00=_mm256_add_ps(c00,c02);
                        c00=_mm256_add_ps(c00,c03);
                        c10=_mm256_add_ps(c10,c11);
                        c10=_mm256_add_ps(c10,c12);
                        c10=_mm256_add_ps(c10,c13);
                      /*  _mm256_storeu_ps(&C[i*b_col+j],c00);
                     _mm256_storeu_ps(&C[(i+1)*b_col+j],c10);
                     _mm256_storeu_ps(&C[i*b_col+(j+1)],c01);
                     _mm256_storeu_ps(&C[(i+1)*b_col+(j+1)],c11);
                     _mm256_storeu_ps(&C[i*b_col+(j+2)],c02);
                     _mm256_storeu_ps(&C[(i+1)*b_col+(j+2)],c12);
                     _mm256_storeu_ps(&C[i*b_col+(j+3)],c03);
                     _mm256_storeu_ps(&C[(i+1)*b_col+(j+3)],c13);
                     __m256 cs0=_mm256_setzero_ps();
                    this could be the error it should have stored after k ,
                   */
                     for (;k<k_end;k++){
                        __m256 ar0=_mm256_broadcast_ss(&A[i*a_col+k]);
                        __m256 br0=_mm256_loadu_ps(&B[k*b_col+j]);
                        __m256 ar1=_mm256_broadcast_ss(&A[(i+1)*a_col+k]);
                         c00=_mm256_fmadd_ps(ar0,br0,c00);
                         c10=_mm256_fmadd_ps(ar1,br0,c10);
                     }
                    __m256 c_old=_mm256_loadu_ps(&C[i*b_col+j]);
                    __m256 c_old1=_mm256_loadu_ps(&C[(i+1)*b_col+j]);
                    c00=_mm256_add_ps(c00,c_old);
                    c10=_mm256_add_ps(c10,c_old1);
                    _mm256_storeu_ps(&C[i*b_col+j],c00);
                    _mm256_storeu_ps(&C[(i+1)*b_col+j],c10);
                    // it doesnt make sense still
                    }
                    for (;j<j_end;j++){
                    float acc0 = C[i*b_col+j];
                    float acc1 = C[(i+1)*b_col+j];
                   for(int k=k_t;k<k_end;k++){
                    acc0+=A[i*a_col+k]*B[k*b_col+j];
                    acc1+=A[(i+1)*a_col+k]*B[k*b_col+j];
                }
                    C[i*b_col+j]=acc0;
                    C[(i+1)*b_col+j]=acc1;
            }
            }
               for (;i<i_end;i++){
                 for (int j = j_t; j < j_end; j++) {
                float acc=C[i*b_col+j];
                for (int k=k_t; k < k_end; k++)
                acc +=A[i*a_col+k]*B[k*b_col+j];
                C[i*b_col+j]=acc;
            }      
         }
        }
    }
    }
    return c;
}

// this method is just for testing and how much compiler can optimise 
Tensor Gemm_tiled_scaler(Tensor&a,Tensor&b,int block_size){
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
               for (int i=i_t;i<i_end-1;i+=2){
                    for (int j=j_t;j<j_end-1;j+=2){
                        float c000=0,c001=0,c002=0,c003=0;
                        float c010=0,c011=0,c012=0,c013=0;
                        float c100=0,c101=0,c102=0,c103=0;
                        float c110=0,c111=0,c112=0,c113=0;
                 //       float ak1=A[i*a_col+k]; what can we cache here we can load once might improve
                   //     float ak2=A[(i+1)*a_col+k]; 
                        int k=k_t;
                for (;k<k_end-3;k+=4){
                        float a00=A[i*a_col+k];
                        float a10=A[(i+1)*a_col+k];
                        float b00=B[k*b_col+j];
                        float b01=B[k*b_col+(j+1)];
                        c000+=a00*b00;
                        c010+=a00*b01;
                        c100+=a10*b00;
                        c110+=a10*b01;
                        float a01=A[i*a_col+k+1];
                        float a11=A[(i+1)*a_col+k+1];
                        float b10=B[(k+1)*b_col+j];
                        float b11=B[(k+1)*b_col+(j+1)];
                        c001+=a01*b10;
                        c011+=a01*b11;
                        c101+=a11*b10;
                        c111+=a11*b11;
                        float a02=A[i*a_col+k+2];
                        float a12=A[(i+1)*a_col+k+2];
                        float b20=B[(k+2)*b_col+j];
                        float b21=B[(k+2)*b_col+(j+1)];
                        c002+=a02*b20;
                        c012+=a02*b21;
                        c102+=a12*b20;
                        c112+=a12*b21;
                        float a03=A[i*a_col+k+3];
                        float a13=A[(i+1)*a_col+k+3];
                        float b30=B[(k+3)*b_col+j];
                        float b31=B[(k+3)*b_col+(j+1)];
                        c003+=a03*b30;
                        c013+=a03*b31;
                        c103+=a13*b30;
                        c113+=a13*b31;
                        }
                        float c00 =c000+c001+c002+c003;
                        float c01 =c010+c011+c012+c013;
                        float c10 =c100+c101+c102+c103;
                        float c11 =c110+c111+c112+c113;
                        float a00,a10,b00,b01;
                        for(;k<k_end;++k){
                            a00 = A[i * a_col + k];
                            a10 = A[(i + 1) * a_col + k];
                            b00 = B[k * b_col + j];
                            b01 = B[k * b_col + (j + 1)];
                            c00+=a00*b00;
                            c01+=a00*b01;
                            c10+=a10*b00;
                            c11+=a10*b01;                
                        }
                           C[i * b_col + j]+= c00;
                            C[i * b_col + (j + 1)]+= c01;
                            C[(i + 1) * b_col + j]+= c10;
                            C[(i + 1) * b_col + (j + 1)]+= c11;
               }
                    if ((j_end-j_t)&1){
                        int j=j_end-1;
                        float sum0=0.0f;
                        float sum1=0.0f;
                        for (int k=k_t;k<k_end;k++){
                            float a0=A[i*a_col+k];
                            float a1=A[(i+1)*a_col+k];
                            float b0=B[k*b_col+j];
                            sum0+=a0*b0;
                            sum1+=a1*b0;
                        }
                        C[i*b_col+j]+=sum0;
                        C[(i+1)*b_col+j]+=sum1;
                    }
            }
               if((i_end-i_t)&1){
                    int i=i_end-1;
                    for (int j=j_t;j+1 < j_end; j += 2){
                    float sum0 = 0.0f;
                    float sum1 = 0.0f;
                        for (int k=k_t;k<k_end;k++){
                        float a  = A[i * a_col + k];
                        float b0 = B[k * b_col + j];
                        float b1 = B[k * b_col + (j + 1)];
                        sum0+=a*b0;
                        sum1+=a*b1;
                        }
                        C[i*b_col+j]+= sum0;
                        C[i*b_col+j+1]+= sum1;
               }
               if ((j_end-j_t)&1){
                    int j = j_end - 1;
                    float sum = 0.0f;
                    for (int k = k_t; k < k_end; ++k)
                    sum += A[i * a_col + k] * B[k * b_col + j];
                    C[i * b_col + j] += sum;
                }
            }
        }

        }}
    return c;
}
