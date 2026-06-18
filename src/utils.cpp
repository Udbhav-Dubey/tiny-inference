#include "utils.h"
#include <fstream>
#include <sstream>
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
std::vector<metadata_file>read_metadata_file(){
    std::ifstream mfile("data_set/metadata.txt");
    if (!mfile){
        std::cerr<<"error in loading metadata.txt \n";
        exit(1);
    }
    std::vector<metadata_file>res;
    std::string line{};  
    bool rt_flag{0}; // reading tensor flag;
    metadata_file temp;
    while(std::getline(mfile,line)){
        if (line.starts_with("Tensor")){
            if (rt_flag){
                res.push_back(temp);
            }
            temp = metadata_file{};
            rt_flag=1;
            auto col=line.find(':');
            std::string tensor_name=line.substr(col+1);
            tensor_name.erase(0,tensor_name.find_first_not_of(" \t"));
            temp.name=tensor_name;
            auto dot=tensor_name.find('.');
            if (dot!=std::string::npos){
                temp.nnum=tensor_name.substr(0,dot);
                temp.type=tensor_name.substr(dot+1);
            }
        }
        else if (line.starts_with("Shape")){
            auto lb=line.find('[');
            auto rb=line.find(']');
            std::string shape_str=line.substr(lb+1,rb-lb-1);
            auto comma=shape_str.find(',');
            if (comma!=std::string::npos){
                int rows=std::stoi(shape_str.substr(0,comma));
                int cols=std::stoi(shape_str.substr(comma+1));
                temp.shape={rows,cols};
            }
            else {
                int rows=std::stoi(shape_str);
                temp.shape={rows};
            }
        }

    }
    if (rt_flag){res.push_back(temp);}
    return res;
}
void check_mfiles(std::vector<metadata_file>&mfiles){
 for (auto m:mfiles){
        std::cout << "\nname  : " << m.name << "\n";
        std::cout << "n num : " << m.nnum << "\n";
        std::cout << "type : " << m.type << "\n";
        std::cout << "shape : ";
        for (int i:m.shape){
            std::cout << i << " ";
            }
    }
}
