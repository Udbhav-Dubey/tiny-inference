#include "tensor.h"
#include <iostream>
#include <cassert>
// the current implementation would have somebody insert things after few modules input pipeline would be genrated that converts image to tensor directly currently tester will manually input or the tester.cpp would 
// btw what is unit test figure it out 
// many of the methods would be changed overtime  
// 
// for now consider default constructor to be closed , for now only we to make object would be giving dimensions
/*Tensor::Tensor(){
// default constructor should have something too    
    //std::cout << "enter row and column to proceed\n";
    std::cin>>size2d.first;
    std::cin>>size2d.second;
    stride_calc();
    set_mem_array();
}*/
Tensor::Tensor(int row,int col){
    size2d={row,col};
    mem_array.assign(row*col,0);
    stride_calc();
    //set_mem_array();
}
void Tensor::stride_calc(){
    stride={size2d.second,1};
}
// currently its cin depending on better altlernative as module progresses withing setting the mem_array 
// one of the possible ways is to send a ds and copy from it but i think thats bad memory wise , consider anything wanting to deal with setting mem_array should be able to send in its stream , like in pipe in or something 
void Tensor::set_mem_array_mt(){
    //std::cout << "fill in row by row \n";
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
float Tensor:: get_val(int i){
    assert(i<(size2d.first*size2d.second));
    return mem_array[i];
}
float Tensor::get_val(int i,int j){
    int temp=(i*size2d.second+j);
    assert(temp<(size2d.first*size2d.second));
    return mem_array[temp];
}
void Tensor::set_val(int i,float value){
     assert(i<(size2d.first*size2d.second));
    mem_array[i]=value;

}
void Tensor::set_val(int i,int j,float value){
     int temp=(i*size2d.second+j);
    assert(temp<(size2d.first*size2d.second));
    mem_array[temp]=value;
}
int Tensor::grow()const{
    return size2d.first;
}
int Tensor::gcol()const{
    return size2d.second;
}
float* Tensor::data(){
    return mem_array.data();
}
