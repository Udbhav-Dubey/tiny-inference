#include <iostream>
#include <string>
#include "pth_converter.h"
#include "utils.h"
#include "image_to_tensor.h"
#include <cmath>
#include "sequence.h"
#include "tensor.h"
#include "linear.h"
#include "relu.h"
#include "Layer.h"
#include "prediction.h"
#include <cassert>
#include <limits>
std::string artBlock = R"(
 _   __      _                   _   _                                             
| | / /     | |                 | | ( )                                            
| |/ /  __ _| | ____ _ _ __ ___ | |_|/ ___                                         
|    \ / _` | |/ / _` | '__/ _ \| __| / __|                                        
| |\  \ (_| |   < (_| | | | (_) | |_  \__ \                                        
\_| \_/\__,_|_|\_\__,_|_|  \___/ \__| |___/                                        
                                                                                   
                                                                                   
 _____ _____ _   ___   __  _____ _   _ ______ ___________ _____ _   _ _____  _____ 
|_   _|_   _| \ | \ \ / / |_   _| \ | ||  ___|  ___| ___ \  ___| \ | /  __ \|  ___|
  | |   | | |  \| |\ V /    | | |  \| || |_  | |__ | |_/ / |__ |  \| | /  \/| |__  
  | |   | | | . ` | \ /     | | | . ` ||  _| |  __||    /|  __|| . ` | |    |  __| 
  | |  _| |_| |\  | | |    _| |_| |\  || |   | |___| |\ \| |___| |\  | \__/\| |___ 
  \_/  \___/\_| \_/ \_/    \___/\_| \_/\_|   \____/\_| \_\____/\_| \_/\____/\____/ 
                                                                                   

)";
void update_data_set(){
    std::string path{};
    std::cout << "enter the path to data_set file\ncurrently supporting .pth only\n";
    std::cout << "press enter to get default(test/mnist_mlp.pth) \n \n";
    std::cin.ignore();
    std::getline(std::cin, path);
    if (path.empty()){path="test/mnist_mlp.pth";}
    Parser par;
    par.extract_dataset(path);
    std::cout << "updated the model in folder dataset\n";
}
void show_options(){
    std::cout << "\n\nplease select an option to proceed\n";
    std::cout << "1 : run the simple MNIST on current data set\n";
    std::cout << "2 : update the model " << "\n"; 
    std::cout << "3 : quit \n";
}
void run_model(){
    std::vector<metadata_file> mfiles=read_metadata_file();
    //check_mfiles(mfiles);
    int mfiles_size=mfiles.size();
    std::cout<<"enter the image path\n";
    std::string ipath;
    std::cin >>ipath;
    int res=mfiles[0].shape.back();
    res=std::sqrt(res);
    image input(ipath,res,res);
    Tensor in=input.get_input();
    sequence seq;
    assert(mfiles.size()%2==0&&"the dataset must have weight bias pair to work\n");
    for (int i=0;i<mfiles_size;i+=2){
        assert(mfiles[i].type=="weight"&&"expected weight at even index\n");
        assert(mfiles[i+1].type=="bias"&&"expected bias at odd index\n");
        assert(mfiles[i].shape[0]==mfiles[i+1].shape[0]&&"weight ouptut features should meet bias size\n");
        std::string wpath="data_set/"+mfiles[i].type+mfiles[i].nnum+".txt";
        std::string bpath="data_set/"+mfiles[i+1].type+mfiles[i+1].nnum+".txt";
        seq.add(new Linear(mfiles[i].shape[1],mfiles[i].shape[0],wpath,bpath));
        if (i+2<mfiles.size()){
            seq.add(new Relu);
        }
    }
    Tensor output=seq.forward(in);
    std::cout << "currently using gemm_tiled\n";
    std::cout << "final ans : " << prediction(output) << "\n";
}
void ask_options(){
    int option{};
    while(true){
        bool flag{0};
        if (!(std::cin>>option)){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
            std::cout<<"\ninvalid input,enter a number only\n";
            show_options();
            continue;
        }
        switch(option){
            case 1 :  // run the model;
                      run_model();
                      break;
            case 2 :  // update the model;
                    update_data_set();
                      break;
            case 3 :  flag=1;
                      break;
            default : std::cout << "\nwrong option\n";
        }
        if (flag==1){
            break;
        }
        show_options();
}
}

int main (){
    std::cout << artBlock << "\n";
    show_options();
    ask_options();
    return 0;
}
