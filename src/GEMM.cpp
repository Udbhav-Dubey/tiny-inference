#include "GEMM.h"
#include <cassert>
Tensor Gemm(Tensor&a,Tensor&b){
    assert(a.gcol()==b.grow()&&"need to get a.col==b.row equal for matrix multiply");
            // i considered letting a and b be interswitched if b.col was equal to a.row instead of current situation but it is now a rigid api that wants in one direction only , i dont know whats better enginnering being rigid or working with any needs to decide 
    Tensor c(a.grow(),b.gcol());
    for (int i=0;i<a.grow();i++){
         for (int k=0;k<a.gcol();k++){
             float ak=a.get_val(i,k);
            for (int j=0;j<b.gcol();j++){
                c.set_val(i,j,c.get_val(i,j)+(ak*b.get_val(k,j)));
            }
         }
    }
    return c;
}
Tensor Gemm_ijk(Tensor&a,Tensor&b){
    assert(a.gcol()==b.grow()&&"need to get a.col==b.row equal for matrix multiply");
    Tensor c(a.grow(),b.gcol());
        for (int i=0;i<a.grow();i++){
            for (int j=0;j<b.gcol();j++){
                float sum=0;
                for (int k=0;k<a.gcol();k++){
                    sum+=a.get_val(i,k)*b.get_val(k,j);
                }
                c.set_val(i,j,sum);
            }
        }
        return c;
    }
