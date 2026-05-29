#include "../src/tensor.h"
#include <iostream>
int main (){
    std::cout << "hi\n";
    Tensor t1(3,4);
    std::vector<float>tf=t1.get_mem_array();
    std::cout << "printing the mem_array() \n";
    std::pair<int,int>sze=t1.get_size2d();
    for (int i=0;i<tf.size();i++){
        if (i%sze.second==0){std::cout << "\n";}
        std::cout << tf[i] << " ";
    }
    return 0;
}
