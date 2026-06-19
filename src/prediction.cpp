// final input must be size 10 this is in architecure of the engine , if something changes please notify and change the code 
// currently if same prediction that is not dealt with currently return the first one 
#include "prediction.h"
#include <cassert>
#include <algorithm>
#include <iostream>
#include <functional>
#include <vector>
#include <climits>
int prediction(Tensor&input){
    int size=input.gsize();
    assert(size==10&&"the final tensor must be size of 10 to get output\n");
    int ans{0};
    float*in=input.data();
    float maxdata{in[0]};
    for (int i=1;i<10;i++){
        if (in[i]>maxdata){
           ans=i;
           maxdata=in[i];
        }
    }
    return ans;
}
void show_top3(Tensor&input){
    int size=input.gsize();
    assert(size==10&&"the final tensor must be size of 10 to get output\n");
    std::vector<std::pair<float,int>>vals;
    float*in=input.data();
    for (int i=0;i<10;i++){
        vals.push_back({in[i],i});        
    }
    sort(vals.begin(),vals.end(),std::greater<std::pair<float,int>>());
    std::cout << "\ntop 3 predictions are \n";
    std::cout << "\nnumber " << ": logits\n";
    for (int i=0;i<3;i++){
        std::cout << "     "<<vals[i].second << " : " << vals[i].first << "\n"; 
    }

}

