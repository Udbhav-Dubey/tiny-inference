#pragma once
#include "tensor.h"
#include "Layer.h"
class Sigmoid:public Layer{
    public:
        Tensor forward(Tensor&);
};
