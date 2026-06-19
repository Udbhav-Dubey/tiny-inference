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
#include <chrono>
#include <filesystem>
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
std::string d_path{"test_data/mnist_mlp.pth"};
void update_data_set(){
    std::string path{};
    std::cout << "enter the path to data_set file\ncurrently supporting .pth only\n";
    std::cout << "press enter to get default(test_data/mnist_mlp.pth) \n \n";
    std::cin.ignore();
    std::getline(std::cin, path);
    if (path.empty()){path="test_data/mnist_mlp.pth";}
    d_path=path;
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
    auto start_total=std::chrono::high_resolution_clock::now();
    int res=mfiles[0].shape.back();
    res=std::sqrt(res);
    auto start_image=std::chrono::high_resolution_clock::now();
    image input(ipath,res,res);
    Tensor in=input.get_input();
    auto end_image=std::chrono::high_resolution_clock::now();
    auto duration_image=std::chrono::duration_cast<std::chrono::microseconds>(end_image-start_image);
    sequence seq;
    assert(mfiles.size()%2==0&&"the dataset must have weight bias pair to work\n");
    int linear_count{},Relu_count{};
    auto start_loadModel=std::chrono::high_resolution_clock::now();
    long long total_parameters{};
    for (int i=0;i<mfiles_size;i+=2){
        assert(mfiles[i].type=="weight"&&"expected weight at even index\n");
        assert(mfiles[i+1].type=="bias"&&"expected bias at odd index\n");
        assert(mfiles[i].shape[0]==mfiles[i+1].shape[0]&&"weight ouptut features should meet bias size\n");
        std::string wpath="data_set/"+mfiles[i].type+mfiles[i].nnum+".txt";
        std::string bpath="data_set/"+mfiles[i+1].type+mfiles[i+1].nnum+".txt";
        seq.add(new Linear(mfiles[i].shape[1],mfiles[i].shape[0],wpath,bpath));
        total_parameters+=mfiles[i].numel +mfiles[i+1].numel;
        linear_count++;
        if (i+2<mfiles.size()){
            seq.add(new Relu);
            Relu_count++;
        }
    }
    auto end_loadModel=std::chrono::high_resolution_clock::now();
    auto duration_loadModel=std::chrono::duration_cast<std::chrono::microseconds>(end_loadModel-start_loadModel);
    auto start_inference=std::chrono::high_resolution_clock::now();
    Tensor output=seq.forward(in);
    auto end_inference=std::chrono::high_resolution_clock::now();
    auto duration_inference=std::chrono::duration_cast<std::chrono::microseconds>(end_inference-start_inference);
    auto end_total=std::chrono::high_resolution_clock::now();
    auto duration_total=std::chrono::duration_cast<std::chrono::microseconds>(end_total-start_total);
    std::filesystem::path m(d_path);
    std::cout << "\nINFERENCE REPORT -> \n";
    std::cout << "\nModel Name : " << m.filename() ;
    std::cout << "\nInput shape : " << "1 X " << res*res ;
    std::cout << "\nTotal parameters : "<< total_parameters;
    std::cout << "\nLayers : " << "Linear = " << linear_count << " and Relu = " << Relu_count ;
    std::cout << "\nGEMM : " << "tiled(256)\n";
    show_top3(output);
    std::cout << "\nfinal ans : " << prediction(output) << "\n";
    std::cout << "TIME TAKEN : \n";
    std::cout << "image -> tensor : " << duration_image.count() << " microseconds\n";
    std::cout << "loading model   : " << duration_loadModel.count() << " microseconds\n";
    std::cout << "inference       : " << duration_inference.count() << " microseconds\n";
    std::cout << "total time      : " << duration_total.count() << " microseconds\n";
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
