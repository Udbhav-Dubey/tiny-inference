#pragma once
#include "tensor.h"
#include <string>
#include <fstream>
#include <vector>
#include<iostream>
struct metadata_file{
    std::string name{};
    std::string nnum{};
    std::vector<int>shape;
    int numel{};
    std::string type{};
};
Tensor load_data(std::string&filepath);
Tensor load_data_transposed(std::string&filepath);
std::vector<metadata_file>read_metadata_file();
void check_mfiles(std::vector<metadata_file>&mfiles);
template<typename T>
T read (std::ifstream&file){
    T value;
    file.read(reinterpret_cast<char*>(&value),sizeof(T));
    return value;
}
