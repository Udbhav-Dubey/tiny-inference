#include "../src/image_to_tensor.h"
#include <iostream>
#include <vector>
int main (){
    image i("test/test_image.jpg",28,28);
    std::vector<float>d=i.get_input();
    for(int i=0;i<d.size();i++){
        std::cout << d[i] << "  ";
    }
std::cout << d.size() << '\n';
    return 0;
}
