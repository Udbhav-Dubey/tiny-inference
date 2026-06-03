#pragma once 
#include "tensor.h"
class Linear{
    int in_feat;
    int out_feat;
    Tensor Weight;
    Tensor bias;
    public:
    Linear()=delete;
    Linear(int ,int );
    Tensor forward(Tensor&);
};
