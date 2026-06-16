#include "pth_converter.h"
#include <iostream>
EOCD read_eocd(std::string & pth_f){
    std::ifstream file(pth_f,std::ios::binary);
    if (!file){
        std::cerr<<"failed to open file\n";
        exit(1);
    }
    file.seekg(0,std::ios::end);
    std::streamoff fileSize=file.tellg();
    file.seekg(fileSize-22);
    EOCD eocd;
    eocd.signature=read<uint32_t>(file);
    if(eocd.signature != 0x06054B50){
    std::cerr << "EOCD signature invalid\n";
    exit(1);
    }
    eocd.diskNumber=read<uint16_t>(file);
    eocd.centralDirDisk=read<uint16_t>(file);
    eocd.entryThisDisk=read<uint16_t>(file);
    eocd.totalEntry=read<uint16_t>(file);
    eocd.centralDirSize=read<uint32_t>(file);
    eocd.centralDirOffset=read<uint32_t>(file);
    eocd.comlength=read<uint16_t>(file);
    
    return eocd;
}
