#include "../src/image_to_tensor.h"
#include <iostream>
#include <vector>
#include "../src/tensor.h"
int main (){
    image i("test/test_image.jpg",28,28);
    Tensor in=i.get_input();
    float*d=in.data();
    int s=in.gsize();
    for(int i=0;i<s;i++){
        std::cout << d[i] << "  ";
    }
std::cout << s << '\n';
std::cout <<in.grow()<<"\n";
std::cout <<in.gsize()<<"\n";
    return 0;
}
