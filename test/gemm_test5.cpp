#include <iostream>
#include "../src/GEMM.h"
#include "../src/tensor.h"
#include <vector>
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
    Tensor C1 = Gemm_ijk(A,B);
    Tensor C2=Gemm_simd(A,B);
    std::cout << "c1:\n";
    printit(C1);
    std::cout << "c2 : \n";
    printit(C2);
 return 0;
}
