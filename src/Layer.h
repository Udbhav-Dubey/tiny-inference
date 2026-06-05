#pragma once
#include "tensor.h"
class Layer{
    public:
        virtual Tensor forward(Tensor&)=0;
        virtual ~Layer()=default;
};
