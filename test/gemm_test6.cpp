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
    int a1=37;
    int a2=53;
    int a3=51;
        Tensor A(a1,a2);
    for (int i=0;i<a1;i++){
       for (int j=0;j<a2;j++){
        A.set_val(i,j,i+j);
       } 
    }
    Tensor B(a2,a3);
     for (int i=0;i<a2;i++){
       for (int j=0;j<a3;j++){
        B.set_val(i,j,(i+j)%7);
       } 
    }
    Tensor C1 = Gemm_ijk(A,B);
 //   std::cout << "here\n";
    Tensor C2=Gemm_tiled_simd(A,B,16);
    std::cout << "c1:\n";
    printit(C1);
    std::cout << "c2 : \n";
    printit(C2);
   std::vector<float>tf=C2.get_mem_array();
    std::cout << "printing the mem_array() \n";
    std::pair<int,int>sze=C2.get_size2d();
    float max_error = 0.0f;
    for (int i=0;i<tf.size();i++){
        max_error = std::max(max_error, std::abs(C1.get_val(i) - C2.get_val(i)));
        std::cout << max_error << '\n';
      //  if (i%sze.second==0){std::cout << "\n";}
        if (C1.get_val(i)!=C2.get_val(i)){
            std::cout << "Mismatched at " << i << "      ";
        }
  //      std::cout << tf[i] << " ";
    }
   return 0;
}
