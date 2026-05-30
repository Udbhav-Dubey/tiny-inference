#include <iostream>
#include "../src/GEMM.h"
#include "../src/tensor.h"
#include <vector>
int main (){
    Tensor A(2,3);
    A.set_mem_array();
    Tensor B(4,2);
    B.set_mem_array();
    Tensor C = Gemm(A,B);
    std::vector<float>tf=C.get_mem_array();
    std::cout << "printing the mem_array() \n";
    std::pair<int,int>sze=C.get_size2d();
    for (int i=0;i<tf.size();i++){
        if (i%sze.second==0){std::cout << "\n";}
        std::cout << tf[i] << " ";
    }
    return 0;
}

