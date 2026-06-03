#include "../src/linear.h"
#include "../src/tensor.h"
#include "../src/GEMM.h"
#include <iostream>
int main (){
   Linear fc(3,2);
   Tensor input(5,3);
   float *in=input.data();
   for (int i=0;i<15;i++){
        in[i]=1.0f;
   }
   Tensor output=fc.forward(input);
   int row=output.grow();
   int col=output.gcol();
    std::cout << "row " << row << "\n";
    std::cout << "col " << col << "\n";
    for (int i=0;i<row;i++){
        for (int j=0;j<col;j++){
            std:: cout << output.get_val(i,j) << " ";
        }
        std:: cout << "\n";
    }
return 0;
}
