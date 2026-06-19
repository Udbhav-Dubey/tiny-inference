#include "../src/relu.h"
#include "../src/tensor.h"
#include <iostream>
void printit(float*x){
    for (int i=0;i<12;i++){
        if (i%4==0){std::cout << "\n";}
        std::cout << x[i] << " ";
    }
    std::cout << " \n";
}
int main (){
    Tensor t1(3,4);
    float*x=t1.data();
    for (int i=0;i<12;i++){
        if (i%2){x[i]=-7-i;}
        else {x[i]=7+i;}
    }
    std::cout << "before relu " << "\n";
    printit(x);
    Relu r;
    r.forward(t1);
    printit(x);
    return 0;
}
