#pragma once
#include <vector>
#include <utility>
class Tensor{
    private:
        std::vector<float>mem_array; // should i take it float* mem_array due to performance ?
        std::pair<int,int>stride;
        std::pair<int,int>size2d;
        // input keliye conversion kese hoga?
        // input->mem_array->operations
    public:
        Tensor();
        Tensor(int,int);
    void set_mem_array();
    void stride_calc();
    std::vector<float> get_mem_array();
    std::pair<int,int> get_stride();
    std::pair<int,int> get_size2d();
    Tensor(const Tensor&)=delete;
    Tensor &operator=(const Tensor&)=delete;
};
