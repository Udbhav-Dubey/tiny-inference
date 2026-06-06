#include "linear.h"
#include "tensor.h"
#include "GEMM.h"
#include "utils.h"
#include <cassert>
#include <iostream>
/*void print_val(Tensor&x){
    std::cout << "\n";
    int size=x.gsize();
    float*X=x.data();
    for (int i=0;i<size;i++){
        std::cout << X[i] << "\n";
    }
}*/
Linear::Linear(int in,int out,std::string &wpath,std::string &bpath):in_feat{in},out_feat{out},Weight(in,out),bias(1,out_feat) {
    Weight=load_data(wpath);
    assert(Weight.grow()==in &&"weights row should match input features\n");
    assert(Weight.gcol()==out&&"weights col should match output features\n");
//    print_val(Weight);
    bias=load_data(bpath);
  //  print_val(bias);
    assert(bias.gcol()==out &&"bias col should match output features\n");
}
Tensor Linear::forward(Tensor&input){
    int in_row=input.grow();
    int in_col=input.gcol();
    assert(in_col==in_feat&&"the col of input should be equal to row of weights for multiplication");
   // print_val(Weight);
    //print_val(input);
    Tensor Out(Gemm(input,Weight));
    // for now lets do Gemm normal but we would need to figure the best one out in 3 options 
    float*o=Out.data();
    float*b=bias.data();
    for (int i=0;i<in_row;i++){
        for (int j=0;j<out_feat;j++){
            o[i*out_feat+j]=o[i*out_feat+j]+b[j];
        }
    }
    return Out;
}
