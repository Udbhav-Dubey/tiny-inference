#include "../src/linear.h"
#include "../src/tensor.h"
#include "../src/GEMM.h"
#include "../src/relu.h"
#include "../src/Layer.h"
#include <iostream>
#include <../src/sequence.h>
int main (){
  // Linear fc(3,2);
   Tensor input(5,3);
   float *in=input.data();
   for (int i=0;i<15;i++){
        if (i%2){in[i]=-7-i;}
        else {in[i]=7+i;}
   }
   std::string wpath="../data_set/weights.txt";
    std::string bpath="../data/set/bias.txt";
   sequence seq;
   seq.add(new Linear(3,2,wpath,bpath));
   // Layer*l1,*l2;
    //l1=new Linear (3,2);
    //Tensor output=l1->forward(input);
   //Tensor output=fc.forward(input);
   //int row=output.grow();
   //int col=output.gcol();
   /* std::cout << "row " << row << "\n";
    std::cout << "col " << col << "\n";
    for (int i=0;i<row;i++){
        for (int j=0;j<col;j++){
            std:: cout << output.get_val(i,j) << " ";
        }
        std:: cout << "\n";
    }*/
    std::cout << "now putting this through relu \n";
    //l2=new Relu ;
    //Tensor out=l2->forward(output);
    seq.add(new Relu);
 //   Relu r;
  //  r.forward(output);
  Tensor out =seq.forward(input);
   int row=out.grow();
   int col=out.gcol();
   for (int i=0;i<row;i++){
        for (int j=0;j<col;j++){
            std:: cout << out.get_val(i,j) << " ";
        }
        std:: cout << "\n";
    }
   std::cout << out.grow() << " "
          << out.gcol() << "\n";
    //delete l1;
    //delete l2;
return 0;
}
