#include <iostream>
#include <string>
#include "pth_converter.h"
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
void ask_options(){
    int option{};
    while(true){
        bool flag{0};
        std::cin>>option;
        switch(option){
            case 1 :  // run the model;
                std::cout << "\nwill run the model here\n";
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
