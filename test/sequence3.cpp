#include "../src/sequence.h"
#include "../src/tensor.h"
#include "../src/Layer.h"
#include "../src/linear.h"
#include "../src/relu.h"
#include "../src/pth_converter.h"
#include "../src/image_to_tensor.h"
#include "../src/prediction.h"
#include <iostream>
#include <string>
void printit(Tensor&x){
    int x_col=x.gcol();
    int x_row=x.grow();
    float*X=x.data();
    for (int i=0;i<x_col*x_row;i++){
        if (i!=0&&i%x_col==0)std::cout << "\n";
        std::cout << X[i] << " ";
    }
}
void printit(float*x){
    for (int i=0;i<24;i++){
        if (i%4==0){std::cout << "\n";}
        std::cout << x[i] << " ";
    }
    std::cout << " \n";
}
int main (){
    // input
    //
    std::cout << "input is \n";
    printit(in);
    sequence seq;
    std::string wpath0="../data_set/weights.txt";
    std::string bpath0="../data/set/bias.txt";
    std::string wpath2="../data/set/weights.txt";
    std::string bpath2="../data/set/bias2.txt";
    // weights loading 
    seq.add(new Linear (4,5,wpath,bpath));
    seq.add(new Relu);
    seq.add(new Linear (5,3,wpath,bpath));
    std::cout << "output is \n";
    // processing 
    Tensor output=seq.forward(input);
    printit(output);
    // idhar prediction aayegi
    return 0;
}
