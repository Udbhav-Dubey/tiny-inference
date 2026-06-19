#include "../src/sequence.h"
#include "../src/tensor.h"
#include "../src/Layer.h"
#include "../src/linear.h"
#include "../src/relu.h"
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
    Tensor input(6,4);
    float*in=input.data();
    for (int i=1;i<24;i++){
        if (i%3==0){
            in[i]=-100+i;
        }
        else {
            in[i]=100+i;
        }
    }
    std::cout << "input is \n";
    printit(in);
    sequence seq;
    std::string wpath="../data_set/weights.txt";
    std::string bpath="../data/set/bias.txt";
    seq.add(new Linear (4,5,wpath,bpath));
    seq.add(new Relu);
    seq.add(new Linear (5,3,wpath,bpath));
    std::cout << "output is \n";
    Tensor output=seq.forward(input);
    printit(output);
    return 0;
}
