#pragma once
#include "tensor.h"
#include "Layer.h"
class Relu:public Layer{
    public:
        Tensor forward(Tensor&);
};
