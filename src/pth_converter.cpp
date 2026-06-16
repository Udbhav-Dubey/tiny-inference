#include "pth_converter.h"
#include <iostream>
EOCD Parser::read_eocd(std::string & pth_f){
    file.open(pth_f,std::ios::binary);
    if (!file){
        std::cerr<<"failed to open file\n";
        exit(1);
    }
    file.seekg(0,std::ios::end);
    std::streamoff fileSize=file.tellg();
    file.seekg(fileSize-22);
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
    
    /* test central directory 
 file.seekg(eocd.centralDirOffset);
uint32_t sig = read<uint32_t>(file);
std::cout<< "Central Dir Signature: 0x" << std::hex << sig << '\n';
*/    
return eocd;
}
std::vector<zfiles> Parser::parse_central_directory(){
    file.seekg(eocd.centralDirOffset); 
    std::vector<zfiles>res(eocd.totalEntry);
    for (int i=0;i<eocd.totalEntry;i++){
        zfiles zf;
        zf.signature=read<uint32_t>(file);
        read<uint16_t>(file);// version made by
        read<uint16_t>(file);
        read<uint16_t>(file); 
        read<uint16_t>(file);
        read<uint16_t>(file);
        read<uint16_t>(file);
        read<uint32_t>(file);
        read<uint32_t>(file);
        zf.uncompressed_Size=read<uint32_t>(file);
        zf.namesize=read<uint16_t>(file);
        uint32_t el=read<uint16_t>(file);
        uint32_t cl=read<uint16_t>(file);
        read<uint16_t>(file);
        read<uint16_t>(file);
        read<uint32_t>(file);
        zf.localheaderOffset=read<uint32_t>(file);
        std::string filename(zf.namesize,'\0');
        file.read(filename.data(),filename.length());
        file.seekg(el,std::ios::cur);
        file.seekg(cl,std::ios::cur);
        zf.filename=filename;
        res[i]=zf;
    }
    return res;
}   
