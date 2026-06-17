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
        zf.extra_length=el;
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
        zf.dataOffset=zf.localheaderOffset+30+zf.namesize+zf.extra_length;
        res[i]=zf;
    }
    zip_files=res;
    return res;
}  
std::string Parser::read_binunicode(size_t &pos){
    uint32_t len=bytes[pos]|(bytes[pos+1]<<8)|(bytes[pos+2]<<16)|(bytes[pos+3]<<24);
    pos+=4;
    std::string s;
    for (uint32_t i=0;i<len;i++){
        s.push_back(bytes[pos+i]);
    }
    pos+=len;
    return s;
}
int Parser::read_binint1(size_t &pos){
    return bytes[pos++];
}
int Parser::read_binint2(size_t &pos){
    int v=bytes[pos]|(bytes[pos+1]<<8);
    pos+=2;
    return v;
}
int Parser::read_binint(size_t &pos){
    int v=bytes[pos]|(bytes[pos+1]<<8)|(bytes[pos+2]<<16)|(bytes[pos+3]<<24);
            pos+=4;
            return v;
}
void Parser::create_metadata(std::string &filename){
    zfiles pickle;
   for (auto &zf:zip_files){
        if (zf.filename.find("data.pkl")!=std::string::npos){
            pickle=zf;
            break;
        }
   }
    file.seekg(pickle.localheaderOffset);
    uint32_t sig=read<uint32_t>(file);
    if (sig!=0x04034B50){
        std::cerr<<"error in signature matching some data curroption happened\n";
        exit(1);
    }
    file.seekg(pickle.dataOffset);
    bytes.resize(pickle.uncompressed_Size);
    file.read(reinterpret_cast<char*>(bytes.data()),pickle.uncompressed_Size);
    std::vector<token>tokens;
    size_t pos=0;
    while(pos<bytes.size()){
        uint8_t op=bytes[pos++];
        switch(op){
case PROTO : {pos+=1;break;}
case  GLOBAL : {
        while(pos<bytes.size()&&bytes[pos]!='\n'){pos++;}
        pos++;
        while(pos<bytes.size()&&bytes[pos]!='\n'){pos++;}
        pos++;
        break;
       }
        case BINUNICODE:{
            std::string s=read_binunicode(pos);
            token t;
            t.type=token::STRING;
            t.s=s;
            tokens.push_back(t);
            break;
             }
        case BININT1:{
            int v=read_binint1(pos);
            token t;
            t.type=token::INT;
            t.v=v;
            tokens.push_back(t);
            break;
            }
        case BININT2:{
             int v=read_binint2(pos);
            token t;
            t.type=token::INT;
            t.v=v;
            tokens.push_back(t);
            break;
                 }
        case BININT :{
             int v=read_binint(pos);
            token t;
            t.type=token::INT;
            t.v=v;
            tokens.push_back(t);
            break;
                 }
        case  BINPUT:
        case BINGET: {pos+=1;break;}
        case LONG_BINPUT :
        case LONG_BINGET: {pos+=4;break;}
         case MARK:
         case EMPTY_TUPLE:
         case EMPTY_DICT:
         case EMPTY_LIST:
         case REDUCE:
         case BUILD:
         case TUPLE1:
         case TUPLE2:
         case TUPLE3:
         case SETITEM:
         case SETITEMS:
         case APPENDS:
         case NONE:
         case NEWTRUE:
         case NEWFALSE:
        case STOP :{break;}
        default:{
        std::cerr<<"opcode not in list \n opcode is " << std::hex << (int)op<< "at pos " << std::dec << (pos-1) <<"\n";
            break;
                }

        } 
}
    // debug — print all tokens so you can figure out offsets
    for (size_t i = 0; i < tokens.size(); i++){
        if (tokens[i].type == token::STRING)
            std::cout << "[" << i << "] STRING: " << tokens[i].s << "\n";
        else
            std::cout << "[" << i << "] INT:    " << tokens[i].v << "\n";
    }

   /* while(pos<bytes.size()){
        uint8_t op=bytes[pos++];
        if (op==BINUNICODE){
            std::string s=read_binunicode(pos);
            token t;
            t.type=token::STRING;
            t.s=s;
            tokens.push_back(t);
        }
        else if (op==BININT1){
            int v=read_binint1(pos);
            token t;
            t.type=token::INT;
            t.v=v;
            tokens.push_back(t);
        }
        else if (op==BININT2){
            int v=read_binint2(pos);
            token t;
            t.type=token::INT;
            t.v=v;
            tokens.push_back(t);
        }
        else if (op==BININT){
            int v=read_binint(pos);
            token t;
            t.type=token::INT;
            t.v=v;
            tokens.push_back(t);
        }
    }*/
   std::vector<WData>tensordata;
    for (size_t i=0;i<tokens.size();i++){
        if (tokens[i].type!=token::STRING){
            continue;
        }
        std::string &name=tokens[i].s;
        if (name.find(".weight")==std::string::npos&&name.find(".bias")==std::string::npos){continue;}
        WData t;
        t.name=name;
        t.dtype="float32";
        if (name.find(".weight") != std::string::npos){
        if (i + 7 >= tokens.size()) continue;
        size_t id_pos   = (tokens[i+1].s == "storage") ? i+2 : i+1;
        size_t numel_pos = (tokens[i+1].s == "storage") ? i+4 : i+2;  // skip "cpu" if present
        size_t shape_pos = (tokens[i+1].s == "storage") ? i+6 : i+4;
    
        const std::string &sid = tokens[id_pos].s;
        if (sid.empty() || sid.find_first_not_of("0123456789") != std::string::npos) continue;
        t.storageid = std::stoi(sid);
        t.numel     = tokens[numel_pos].v;
        t.shape.push_back(tokens[shape_pos].v);
        t.shape.push_back(tokens[shape_pos+1].v);
}       
        else {
         if (i + 4 >= tokens.size()) continue;
         if (tokens[i+1].type != token::STRING) continue;
         const std::string &sid = tokens[i+1].s;
         if (sid.empty() || sid.find_first_not_of("0123456789") != std::string::npos) continue;
         t.storageid = std::stoi(sid);
         t.numel     = tokens[i+2].v;
         t.shape.push_back(tokens[i+4].v);
}
tensordata.push_back(t);
    }
   /*   for (size_t i=0;i<tokens.size();i++){
        if (tokens[i].type!=token::STRING){
            continue;
        }
        std::string &name=tokens[i].s;
        if (name.find(".weight")==std::string::npos&&name.find(".bias")==std::string::npos){continue;}
        if (i + 7 > tokens.size()) continue;
    if (tokens[i+1].type != token::STRING) continue;
    const std::string &sid = tokens[i+1].s;
    if (sid.empty() || sid.find_first_not_of("0123456789") != std::string::npos) continue;
        WData t;
        t.name=name;
        t.dtype="float32";
        t.storageid=std::stoi(tokens[i+1].s);
        if (name.find("weight")!=std::string::npos){
            t.numel=tokens[i+4].v;
            t.shape.push_back(tokens[i+6].v);
            t.shape.push_back(tokens[i+7].v);
        }
        else {
            t.numel=tokens[i+2].v;
            t.shape.push_back(tokens[i+4].v);
        }
        tensordata.push_back(t);
        
    }*/
    std::ofstream out(filename);
    for (const auto &t:tensordata){
        out << "Tensor : " << t.name << "\n";
        out << "Storage : " << t.storageid << "\n";
        out << "Dtype : " <<  t.dtype << "\n";
        out << "Shape [";
        for (size_t i=0;i<t.shape.size();i++){
            out << t.shape[i];
            if (i+1<t.shape.size()){
                out << ",";
            }
        }
        out << "]\n";
        out << "Numel :" << t.numel << "\n";
    }
}

