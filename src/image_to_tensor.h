#pragma once
#include "../third_party/stb_image.h"
#include <string>
#include <vector>
#include <cstdint>
#include "tensor.h"
class image{
    private:
        std::string path{};
        int w{};
        int h{};
        int ch{};
        int new_w{};
        int new_h{};
        uint8_t* pixels{nullptr};
        std::vector<float>data;
        std::vector<float>gray;
        void resize();
        void grayscale();
        void normalize();
        void crop_to_digit();
        void pad_to_square();  
    public:
        image(std::string,int,int);
        Tensor get_input();
        ~image();
};
