#include <iostream>
#include "../src/GEMM.h"
#include "../src/tensor.h"
#include <vector>
int main (){
    Tensor A(97,123);
  //  A.set_mem_array_mt();
    for (int i=0;i<97;i++){
       for (int j=0;j<123;j++){
        A.set_val(i,j,i+j);
       } 
    }
    Tensor B(123,89);
     for (int i=0;i<123;i++){
       for (int j=0;j<89;j++){
        B.set_val(i,j,(i+j)%7);
       } 
    }

    //B.set_mem_array_mt();
    Tensor C1 = Gemm_ijk(A,B);
    Tensor C2=Gemm_tiled(A,B,32);
    std::vector<float>tf=C2.get_mem_array();
    std::cout << "printing the mem_array() \n";
    std::pair<int,int>sze=C2.get_size2d();
    for (int i=0;i<tf.size();i++){
        if (i%sze.second==0){std::cout << "\n";}
        if (C1.get_val(i)!=C2.get_val(i)){
            std::cout << "Mismatched at " << i << "      ";
        }
  //      std::cout << tf[i] << " ";
    }
    std::cout << C1.get_val(122,122) << "\n";
    std::cout << C2.get_val(122,122) << "\n";
    return 0;
}

