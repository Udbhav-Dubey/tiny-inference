#pragma once 
#include "tensor.h"
#include "Layer.h"
class Linear:public Layer{
    int in_feat;
    int out_feat;
    Tensor Weight;
    Tensor bias;
    public:
    Linear()=delete;
    Linear(int ,int );
    Tensor forward(Tensor&);
};
