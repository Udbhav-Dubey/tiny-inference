#include "../src/GEMM.h"
#include "../src/tensor.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <string>
struct dura{
    std::chrono::nanoseconds two_hundred;
    std::chrono::nanoseconds five_hundred;
    std::chrono::nanoseconds one_k;
    std::chrono::nanoseconds two_k;
};
std::vector<dura>thelp(4);
std::vector<std::vector<dura>>bhelp(2,std::vector<dura>(6));
const int ntgmr=10;// number of times gemm run so we could average it 
int A_row_size{};
int A_col_size{};
int B_row_size{};
int B_col_size{};
void table(){
     int blocks[]={16,32,64,128,256,512};
     std::cout << "\nbasic Gemm: \n";
     std::cout << "\n"<<std::left<<std::setw(12)<<"Method"<<std::setw(15)<<"200*200"<<std::setw(15)<<"500*500"<<std::setw(15)
         <<"1000*1000"<<std::setw(15)<<"2000*2000"<<"\n";
     std::vector<std::string>Name={"IKJ","IJK","SIMD"};
     for (int i=0;i<Name.size();i++){
        std::cout << std::left<<std::setw(12)<<Name[i]<<std::setw(15)<<thelp[i].two_hundred.count()/ntgmr
        <<std::setw(15)<<thelp[i].five_hundred.count()/ntgmr
        <<std::setw(15)<<thelp[i].one_k.count()/ntgmr
        <<std::setw(15)<<thelp[i].two_k.count()/ntgmr<<"\n";
     }
     std::vector<std::string>tiled_names={"SIMPLE TILED ", "SIMD TILED"};
     for (int i=0;i<tiled_names.size();i++){
        std::cout << "\nFor " << tiled_names[i] << " \n\n";
     std::cout << std::left<<std::setw(12)<<"\nMethod"<<std::setw(15)<<"200*200"<<std::setw(15)<<"500*500"<<std::setw(15)
         <<"1000*1000"<<std::setw(15)<<"2000*2000"<<"\n";
            for (int j=0;j<6;j++){
                std::cout << std::left<<std::setw(12)<<blocks[j]<<std::setw(15)<<bhelp[i][j].two_hundred.count()/ntgmr
                <<std::setw(15)<<bhelp[i][j].five_hundred.count()/ntgmr
                <<std::setw(15)<<bhelp[i][j].one_k.count()/ntgmr
                <<std::setw(15)<<bhelp[i][j].two_k.count()/ntgmr<<"\n";
            }
     }
}
void run_multi(int ar,int ac,int br,int bc,int&turn,int block_size=0,int t2i=0){
    A_row_size=ar;
    A_col_size=ac;
    B_row_size=br;
    B_col_size=bc;
    Tensor A(A_row_size,A_col_size);
    Tensor B(B_row_size,B_col_size);
    Tensor C(A_row_size,B_col_size);
    for (int i=0;i<A.grow();i++){
        for (int j=0;j<A.gcol();j++){
            A.set_val(i,j,(i+j)%10);
        }
    }
    for(int i=0;i<B.grow();i++){
        for (int j=0;j<B.gcol();j++){
            B.set_val(i,j,(i+j)%7);
        }
    }
    auto start=std::chrono::high_resolution_clock::now();
    for (int i=0;i<ntgmr;i++){
      if (turn==0){ C=Gemm(A,B); }
      else if (turn==3){C=Gemm_tiled(A,B,block_size);}
      else if (turn==2){C=Gemm_simd(A,B);}
      else if (turn==4){C=Gemm_tiled_simd(A,B,block_size);}
      else {C=Gemm_ijk(A,B);}
    }
    auto end=std::chrono::high_resolution_clock::now();
    auto duration=std::chrono::duration_cast<std::chrono::nanoseconds>(end-start);
    int ind=turn-3;
    if (turn==3||turn==4){
    if(ar==200){
        bhelp[ind][t2i].two_hundred=duration;
    }
    else if (ar==500){
        bhelp[ind][t2i].five_hundred=duration;
    }
    else if (ar==1000){
        bhelp[ind][t2i].one_k=duration;
    }
    else {
        bhelp[ind][t2i].two_k=duration;
    }
    t2i++;
    }
    else {
    if(ar==200){
        thelp[turn].two_hundred=duration;
    }
    else if (ar==500){
        thelp[turn].five_hundred=duration;
    }
    else if (ar==1000){
        thelp[turn].one_k=duration;
    }
    else {
        thelp[turn].two_k=duration;
    }
    }
    auto duration_seconds=std::chrono::duration_cast<std::chrono::seconds>(end-start);
    std::cout << ar << "X" << ac << " * " << br << "X" << bc << " = " << "total : "<<duration.count() << " nanoseconds  average : "<< duration.count()/10 << " nanoseconds \n";
    std::cout << "total : " << duration_seconds.count() << " seconds average : " << duration_seconds.count()/10 << " seconds\n";
    std::cout << "checksum :: " << C.get_val(0,0) << "\n";
}
int main(){
    int turn=1; // if 0 then do ikj if 1 do ijk   if 3 do tiled  if 2 do simd 4 for simd_tiled
/*    std::cout << "for ijk \n";
    run_multi(200,200,200,200,turn);
    run_multi(500,500,500,500,turn);
    run_multi(1000,1000,1000,1000,turn);
    run_multi(2000,2000,2000,2000,turn);
  */  std::cout << "\nfor ikj \n";
    turn=0;
    run_multi(200,200,200,200,turn);
    run_multi(500,500,500,500,turn);
    run_multi(1000,1000,1000,1000,turn);
    run_multi(2000,2000,2000,2000,turn);
/*    turn=2;
    std::cout << "\nfor simd\n";
    run_multi(200,200,200,200,turn);
    run_multi(500,500,500,500,turn);
    run_multi(1000,1000,1000,1000,turn);
    run_multi(2000,2000,2000,2000,turn);
  */  turn =3;
    int idx=0;
    for (int i=16;i<=512;i*=2,idx++){
    std::cout << "\nfor tiled "<< i <<"  block size \n";
    run_multi(200,200,200,200,turn,i,idx);
    run_multi(500,500,500,500,turn,i,idx);
    run_multi(1000,1000,1000,1000,turn,i,idx);
    run_multi(2000,2000,2000,2000,turn,i,idx);
    }
   idx=0;
    turn=4;
    for (int i=16;i<=512;i*=2,idx++){
    std::cout << "\nfor simd tiled "<< i <<"  block size \n";
    run_multi(200,200,200,200,turn,i,idx);
    run_multi(500,500,500,500,turn,i,idx);
    run_multi(1000,1000,1000,1000,turn,i,idx);
    run_multi(2000,2000,2000,2000,turn,i,idx);
    }
    table();
    return 0;
}
