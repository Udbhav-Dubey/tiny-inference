#include "tensor.h"
#include <iostream>
// the current implementation would have somebody insert things after few modules input pipeline would be genrated that converts image to tensor directly currently tester will manually input or the tester.cpp would 
// btw what is unit test figure it out 
// many of the methods would be changed overtime  
Tensor::Tensor(){
// default constructor should have something too    
    std::cout << "enter row and column to proceed\n";
    std::cin>>size2d.first;
    std::cin>>size2d.second;
    stride_calc();
    set_mem_array();
}
Tensor::Tensor(int row,int col){
    size2d={row,col};
    mem_array.resize(row*col);
    stride_calc();
    set_mem_array();
}
void Tensor::stride_calc(){
    stride={size2d.second,1};
}
void Tensor::set_mem_array(){
    std::cout << "fill in row by row \n";
    for (int i=0;i<(size2d.first*size2d.second);i++){
        std::cin>>mem_array[i];
    }
}
std::vector<float>Tensor::get_mem_array(){
    return mem_array;
}
std::pair<int,int> Tensor::get_stride(){
    return stride;
}
std::pair<int,int>Tensor::get_size2d(){
    return size2d;
}
