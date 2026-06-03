#include "linear.h"
#include "tensor.h"
#include "GEMM.h"
#include <cassert>
Linear::Linear(int in,int out):in_feat{in},out_feat{out},Weight(in,out),bias(1,out_feat) {
/* this initialization only for testing will remove with future modules

    float*w=Weight.data();
    for (int i=0;i<in_feat*out_feat;i++){
        w[i]=1.0f;
    }
    float*b=bias.data();
    for (int i=0;i<out_feat;i++){
        b[i]=1.0f;
    }
    */
}
Tensor Linear::forward(Tensor&input){
    int in_row=input.grow();
    int in_col=input.gcol();
    assert(in_col==in_feat&&"the col of input should be equal to row of weights for multiplication");
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
