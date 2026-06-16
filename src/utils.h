#pragma once
#include "tensor.h"
#include <string>
#include <fstream>
Tensor load_data(std::string&filepath);
template<typename T>
T read (std::ifstream&file){
    T value;
    file.read(reinterpret_cast<char*>(&value),sizeof(T));
    return value;
}
