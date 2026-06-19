#include "../src/sequence.h"
#include "../src/tensor.h"
#include "../src/Layer.h"
#include "../src/linear.h"
#include "../src/relu.h"
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
    std::cout << "\n";
}
void printit(float*x){
    for (int i=0;i<4;i++){
        if (i%4==0){std::cout << "\n";}
        std::cout << x[i] << " ";
    }
    std::cout << " \n";
}
int main (){
    Tensor input(1,4);
    float*in=input.data();
    in[0]=1;
    in[1]=2;
    in[2]=3;
    in[3]=4;
    std::cout << "input is \n";
    printit(in);
    sequence seq;
    std::string wpath="data_set/weights1.txt";
    std::string bpath="data_set/bias1.txt";
    std::string wpath2="data_set/weights2.txt";
    std::string bpath2="data_set/bias2.txt";
    seq.add(new Linear (4,3,wpath,bpath));
    seq.add(new Relu);
    seq.add(new Linear (3,10,wpath2,bpath2));
    std::cout << "output is \n";
    Tensor output=seq.forward(input);
    printit(output);
    std::cout << "final ans : " << prediction(output) << "\n";
    return 0;
}
