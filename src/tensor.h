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
        Tensor()=delete;
        Tensor(int,int);
    void set_mem_array_mt(); // mt stands for manual test using cin
    void stride_calc();
    std::vector<float> get_mem_array();
    std::pair<int,int> get_stride();
    std::pair<int,int> get_size2d();
    float get_val(int i); // considering someone asks value in row major 1d 
    float get_val(int i,int j); // considering someone asks value in 2d fashion
    void set_val(int i,float value); // incase something wants to fill the exact thing maybe another module loops overfile themselves and fixes value instead of copying mem array then fixing then setting again
    void set_val(int i,int j,float value); 
    Tensor(const Tensor&)=delete;
    Tensor &operator=(const Tensor&)=delete;
    int grow()const;
    int gcol()const;
    int gsize()const;
    Tensor(Tensor&&)noexcept=default;
    Tensor&operator=(Tensor&&)noexcept=default;
    float*data();

};
