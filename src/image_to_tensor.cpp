#define STB_IMAGE_IMPLEMENTATION
#include "image_to_tensor.h"
#include <iostream>
#include <algorithm>
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
  data.resize(new_w*new_h);
  float scalew=(float)w/new_w;
  float scaleh=(float)h/new_h;
  for (int i=0;i<new_h;i++){
      for (int j=0;j<new_w;j++){
//        int ox=i*scaleh;
//        int oj=j*scalew;
        int r0=i*scaleh, r1=std::min((int)((i+1)*scaleh), h-1);
        int c0=j*scalew, c1=std::min((int)((j+1)*scalew), w-1);
         float sum=0;
         int count=0;
        for(int r=r0;r<=r1;r++)
            for(int c=c0;c<=c1;c++){sum+=gray[r*w+c];count++;}
        data[i*new_w+j]=sum/count;
      }
  }
  w=new_w;
  h=new_h;
}
void image::normalize(){
    for (int i=0;i<w*h;i++){
     data[i]/=255.0f;
     //  data[i]=1.0f-(data[i]/255.0f);
    }
}

Tensor image::get_input(){
    grayscale();
    resize();
    normalize();
    Tensor input(1,h*w);
    float*in=input.data();
    for (int i=0;i<w*h;i++){
        in[i]=data[i];
    }
   return input; 
}
image::~image(){
    stbi_image_free(pixels);
}
