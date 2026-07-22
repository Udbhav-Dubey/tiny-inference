#include "../src/sequence.h"
#include "../src/tensor.h"
#include "../src/Layer.h"
#include "../src/linear.h"
#include "../src/relu.h"
#include <iostream>
#include <string>
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
    for (int i=0;i<64;i++){
        if (i%4==0){std::cout << "\n";}
        std::cout << x[i] << " ";
    }
    std::cout << " \n";
}
void packB(float* B,int k_start,int j_start,int k_size,int j_size,int b_col,float*B_pack){
    for(int k=0;k<k_size;k++){
        for (int j=0;j<j_size;j++){
            B_pack[k*j_size+j]=B[(k_start+k)*b_col+(j_start+j)];
        }
    } 
}
int main (){
    Tensor b(8,8);
    float*B=b.data();
    for (int i=0;i<8;i++){
        for (int j=0;j<8;j++){
            B[(i*8)+j]=8*i+j*9/21;
        }
    }
    printit(b);
    Tensor b_pack(8,8);
    float*B_pack=b_pack.data();
    packB(B,0,0,8,8,8,B_pack);
    printit(b_pack);
    return 0;
}
