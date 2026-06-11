#define STB_IMAGE_IMPLEMENTATION
#include "image_to_tensor.h"
#include <iostream>
image::image(std::string ipath,int nh,int nw){
    path=ipath;
    new_w=nw;
    new_h=nh;
    pixels=stbi_load(ipath.c_str(),&w,&h,&ch,0);
    if (!pixels){
        std::cerr<<"Failed to load image\n";
        exit(1);
    }
}
void image::grayscale(){
    gray.resize(w*h);
    if (ch==1){
       for (int i=0;i<h;i++){
            for (int j=0;j<w;j++){
                gray[(i*w)+j]=(float)pixels[((i*w)+j)];
            }
       } 
    }
    else if (ch==3||ch==4){
    for (int i=0;i<h;i++){
        for (int j=0;j<w;j++){
            int base=((i*w)+j)*ch;
            gray[(i*w)+j]=0.299f*pixels[base+0]+0.587f*pixels[base+1]+0.114f*pixels[base+2];
        }
    }
}
    else {
     std::cerr<<"invalid image type\n";
        exit(1);
    }
ch=1;
}
void image::resize(){
   // 
}
void image::normalize(){
    //
}

std::vector<float> image::get_input(){
    // before making a tensor do 3 things convert to size then grayscale then normalise 
    // then i will just copy into fresh tensor and return ;
    grayscale();
//    resize();
  //  normalize();
    // start making tensor here;
   return gray; 
}
