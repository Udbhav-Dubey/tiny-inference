#include "../src/GEMM.h"
#include "../src/tensor.h"
#include <iostream>
#include <chrono>
const int ntgmr=10;// number of times gemm run so we could average it 
int A_row_size{};
int A_col_size{};
int B_row_size{};
int B_col_size{};
void run_multi(int ar,int ac,int br,int bc){
    A_row_size=ar;
    A_col_size=ac;
    B_row_size=br;
    B_col_size=bc;
    Tensor A(A_row_size,A_col_size);
    Tensor B(B_row_size,B_col_size);
    Tensor C(A_row_size,B_col_size);
    for (int i=0;i<A.grow();i++){
        for (int j=0;j<A.gcol();j++){
            A.set_val(i,j,(i+j)%10);
        }
    }
    for(int i=0;i<B.grow();i++){
        for (int j=0;j<B.gcol();j++){
            B.set_val(i,j,(i+j)%7);
        }
    }
    auto start=std::chrono::high_resolution_clock::now();
    for (int i=0;i<ntgmr;i++){
       C=Gemm(A,B); 
    }
    auto end=std::chrono::high_resolution_clock::now();
    auto duration=std::chrono::duration_cast<std::chrono::nanoseconds>(end-start);
    std::cout << ar << "X" << ac << " * " << br << "X" << bc << " = " << "total : "<< duration.count() << " average : "<< duration.count()/10 << " nanoseconds\n";
    std::cout << "checksum :: " << C.get_val(0,0) << "\n";
}
int main(){
    run_multi(200,200,200,200);
    run_multi(500,500,500,500);
    run_multi(1000,1000,1000,1000);
    return 0;
}
