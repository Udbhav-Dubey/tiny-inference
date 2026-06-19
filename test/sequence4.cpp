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
    for (int i=0;i<784;i++){
        if (i%28==0){std::cout << "\n";}
        std::cout << x[i] << " ";
    }
    std::cout << " \n";
}
int main(){
    std::string ppath = "test/mnist_0_label7.txt";
    Tensor in = load_data(ppath);
    std::cout << "row=" << in.grow() << " col=" << in.gcol() << "\n";

    sequence seq;
    std::string wpath0 = "data_set/weight0.txt";
    std::string bpath0 = "data_set/bias0.txt";
    std::string wpath2 = "data_set/weight2.txt";
    std::string bpath2 = "data_set/bias2.txt";
    seq.add(new Linear(784, 128, wpath0, bpath0));
    seq.add(new Relu);
    seq.add(new Linear(128, 10, wpath2, bpath2));

    Tensor output = seq.forward(in);
    printit(output);
    std::cout << "predicted: " << prediction(output) << " expected: 7\n";
    return 0;
}
