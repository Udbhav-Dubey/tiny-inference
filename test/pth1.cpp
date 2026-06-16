#include "../src/utils.h"
#include "../src/pth_converter.h"
#include <iostream>
#include <string>
int main (){
    EOCD eocd;
    std::string path="test/mnist_mlp.pth";
    eocd=read_eocd(path);
std::cout << "Entries: " << eocd.totalEntry << '\n';
std::cout << "Central Directory Offset: " << eocd.centralDirOffset << '\n';
std::cout << "Central Directory Size: " << eocd.centralDirSize<< '\n';
return 0;
}
