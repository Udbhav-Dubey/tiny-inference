#include "../src/image_to_tensor.h"
#include <iostream>
#include <vector>
#include "../src/tensor.h"
int main (){
    image i("test/test_image.png",28,28);
    Tensor in=i.get_input();
    float*d=in.data();
    int s=in.gsize();
std::cout << s << '\n';
    for (int i = 0; i < 28; i++) {
    for (int j = 0; j < 28; j++) {
        float v = d[i * 28 + j];

        if (v > 100)
            std::cout << '#';
        else if (v > 30)
            std::cout << '*';
        else if (v > 5)
            std::cout << '.';
        else
            std::cout << ' ';
    }
    std::cout << '\n';
}
    float mn = d[0];
    float mx = d[0];
    for (int i = 0; i < s; i++) {
    mn = std::min(mn, d[i]);
    mx = std::max(mx, d[i]);
    }
    std::cout << "min=" << mn << '\n';
    std::cout << "max=" << mx << '\n';
    return 0;
}
