#include "relu.h"
Tensor Relu::forward(Tensor&input){
    float*in=input.data();
    int isize=input.gsize();
    int i_col=input.gcol();
    int i_row=input.grow();
    Tensor Output(i_row,i_col);
    float*out=Output.data();
    for (int i=0;i<isize;i++){
        if (in[i]<0){in[i]=0;}
        out[i]=in[i];
    }
    return Output; 
}
