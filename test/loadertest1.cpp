#include <iostream>
#include "../src/tensor.h"
#include "../src/utils.h"
#include <string>
int main (){
    std::string filepath="data_set/weights.txt";
    Tensor out=load_data(filepath);
    float*o=out.data();
    int col=out.gcol();
    int size=out.gsize();
//    std::cout << out.grow() << out.gcol() << "\n";
    for (int i=0;i<size;i++){
        if (i!=0&&i%col==0){std::cout << "\n";}
        std::cout << o[i] << " ";
    }
    std::cout << "\n";
    return 0;
}

