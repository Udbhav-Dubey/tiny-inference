#include "tanh.h"
#include <cmath>
Tensor tanh::forward(Tensor&input){
    float *in=input.data();
    int isize=input.gsize();
    int i_col=input.gcol();
    int i_row=input.grow();
    Tensor output(i_row,i_col);
    float*out=output.data();
    for (int i=0;i<isize;i++){
        out[i]=tanhf(in[i]);
    }
    return output;
}
