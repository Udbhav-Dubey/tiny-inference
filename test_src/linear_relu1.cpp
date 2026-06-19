#include "../src/linear.h"
#include "../src/tensor.h"
#include "../src/GEMM.h"
#include "../src/relu.h"
#include <iostream>
int main (){
  /* Linear fc(3,2);
   Tensor input(5,3);
   float *in=input.data();
   for (int i=0;i<15;i++){
        if (i%2){in[i]=-7-i;}
        else {in[i]=7+i;}
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
    std::cout << "now putting this through relu \n";
    Relu r;
    Tensor out=r.forward(output);
   for (int i=0;i<row;i++){
        for (int j=0;j<col;j++){
            std:: cout << out.get_val(i,j) << " ";
        }
        std:: cout << "\n";
    }
 */ 
return 0;
}
