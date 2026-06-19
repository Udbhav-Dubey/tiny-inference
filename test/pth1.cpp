#include "../src/utils.h"
#include "../src/pth_converter.h"
#include <iostream>
#include <string>
int main (){
    Parser par;
    std::string path="test/mnist_mlp.pth";
 /*   EOCD eocd=par.read_eocd(path);
std::cout << "Entries: " << eocd.totalEntry << '\n';
std::cout << "Central Directory Offset: " << eocd.centralDirOffset << '\n';
std::cout << "Central Directory Size: " << eocd.centralDirSize<< '\n';
    std::vector<zfiles>res=par.parse_central_directory();
    for (auto x:res){
        std::cout << x.filename << "\n";
    }
    std::string mpath="data_set/metadata.txt";
    par.create_metadata(mpath);
    par.get_weights();*/
    par.extract_dataset(path);
return 0;
}
