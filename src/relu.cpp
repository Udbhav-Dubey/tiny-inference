#include "relu.h"
void Relu::forward(Tensor&input){
    float*in=input.data();
    int isize=input.gsize();
    for (int i=0;i<isize;i++){
        if (in[i]<0){in[i]=0;}
    }
}
