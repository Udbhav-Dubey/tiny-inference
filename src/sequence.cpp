#include "sequence.h"
#include "tensor.h"
void sequence::add(Layer*in){
    layers.push_back(in);
}
int sequence::get_size(){
    return layers.size();
}
Tensor sequence::forward(Tensor&input){
    int in_col=input.gcol();
    int in_row=input.grow();
    int isize=input.gsize();
    float*in=input.data();
    Tensor x(in_row,in_col);
    float *X=x.data();
    for (int i=0;i<isize;i++){
        X[i]=in[i];
    }
    for (int i=0;i<layers.size();i++){
        x=layers[i]->forward(x) ;
    }
    return x;
}
sequence::~sequence(){
    for (auto l:layers){
        delete l;
    }
}
