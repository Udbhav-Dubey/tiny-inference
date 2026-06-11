#include "../src/image_to_tensor.h"
#include <iostream>
#include <vector>
int main (){
    image i("test/test_image.jpg",28,28);
    std::vector<float>d=i.get_input();
std::cout << d.size() << '\n';
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
    for (float x : d) {
    mn = std::min(mn, x);
    mx = std::max(mx, x);
    }
    std::cout << "min=" << mn << '\n';
    std::cout << "max=" << mx << '\n';
    return 0;
}
