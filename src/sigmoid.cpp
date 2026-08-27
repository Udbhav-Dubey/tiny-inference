#include "sigmoid.h"
#include <cmath>
Tensor Sigmoid::forward(Tensor&input){
    float*in=input.data();
    int isize=input.gsize();
    int i_col=input.gcol();
    int i_row=input.grow();
    Tensor Output(i_row,i_col);
    float*out=Output.data();
    for (int i=0;i<isize;i++){
        out[i]=1.0f/(1.0f+expf(-in[i]));
    }
    return output;
}

