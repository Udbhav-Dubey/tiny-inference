#pragma once
#include "Layer.h"
#include "tensor.h"
#include <vector>
class sequence{
    private:
        std::vector<Layer*>layers;
        int size{};
    public:
        void add(Layer*);
        int get_size();
        Tensor forward(Tensor&);
        ~sequence();
};
