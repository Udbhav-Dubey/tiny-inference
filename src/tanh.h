#pragma once
#include "tensor.h"
#include "Layer.h"
class tanh:public Layer{
    public:
        Tensor forward(Tensor&);
};
