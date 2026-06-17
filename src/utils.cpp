#include "utils.h"
#include <fstream>
#include "tensor.h"
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
Tensor load_data(std::string&filepath){
    std::ifstream infile(filepath);
    if (!infile){
        throw std::runtime_error("Failed to open source file " + filepath );
    }
    std::string line{};
    std::vector<float>data;
    std::string token{};
    int row{},col{},total{};
    while(std::getline(infile,line)){
        std::istringstream iss(line);
        row++;
        int col_count{};
        while(iss>>token){
            size_t pos{};
            float num=std::stof(token,&pos);
            if (pos!=token.size()){
                throw std::runtime_error("invalid numeric data: " + token );
            }
            data.push_back(num);
            total++;
            col_count++;
        }
        if (col==0){col=total;} // test does this work
        if (col!=col_count){
            throw std::runtime_error("missing data please look near this row " + std::to_string(row));
        }
    }
    Tensor out(row,col);
    float*o=out.data();
    for (int i=0;i<total;i++){
        o[i]=data[i];
    }
    return out;
} 
Tensor load_data_transposed(std::string&filepath){
     std::ifstream infile(filepath);
    if (!infile){
        throw std::runtime_error("Failed to open source file " + filepath );
    }
    std::string line{};
    std::vector<float>data;
    std::string token{};
    int row{},col{},total{};
    while(std::getline(infile,line)){
        std::istringstream iss(line);
        row++;
        int col_count{};
        while(iss>>token){
            size_t pos{};
            float num=std::stof(token,&pos);
            if (pos!=token.size()){
                throw std::runtime_error("invalid numeric data: " + token );
            }
            data.push_back(num);
            total++;
            col_count++;
        }
        if (col==0){col=total;} // test does this work    
    }
    Tensor out(col, row);
    float *o = out.data();
    for (int r = 0; r < row; r++)
    for (int c = 0; c < col; c++)
        o[c * row + r] = data[r * col + c];
    return out;
}

