#include <fstream>
#include <iostream>
#include <vector>
#include <cstdint>
#include <iomanip>
constexpr uint8_t BINUNICODE = 0x58;
constexpr uint8_t BININT1    = 0x4B;
constexpr uint8_t BININT2    = 0x4D;
constexpr uint8_t BININT     = 0x4A;
std::vector<uint8_t> load_file(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0);
    std::vector<uint8_t> bytes(size);
    file.read(reinterpret_cast<char*>(bytes.data()), size);
    return bytes;
}
std::string read_binunicode(const std::vector<uint8_t>& bytes, size_t& pos){
    uint32_t len =bytes[pos] |(bytes[pos + 1] << 8) | (bytes[pos + 2] << 16) | (bytes[pos + 3] << 24);
    pos += 4;
    std::string s;
    for(uint32_t i = 0; i < len; i++){
        s.push_back(bytes[pos + i]);
    }
    pos += len;
    return s;
}
int read_binint1(const std::vector<uint8_t>& bytes,size_t& pos){
    return bytes[pos++];
}
int read_binint2( const std::vector<uint8_t>& bytes,size_t& pos){
    int v = bytes[pos] | (bytes[pos + 1] << 8);
    pos += 2;
    return v;
}
int read_binint(const std::vector<uint8_t>& bytes, size_t& pos){
    int v = bytes[pos] | (bytes[pos + 1] << 8) | (bytes[pos + 2] << 16) | (bytes[pos + 3] << 24);
    pos += 4;
    return v;
}
int main(){
    auto bytes = load_file("data.pkl");
    size_t pos = 0;
    while(pos < bytes.size()){
        uint8_t op = bytes[pos++];
        if(op == BINUNICODE){
            std::string s =read_binunicode(bytes,pos);
            std::cout<< "STRING: " << s << '\n';
        }
        else if(op == BININT1){
            int v = read_binint1(bytes, pos);
            std::cout<< "INT1: " << v << '\n';
        }
        else if(op == BININT2){
            int v =read_binint2(bytes, pos);
            std::cout<< "INT2: " << v << '\n';
        }
        else if(op == BININT){
            int v =
                read_binint(bytes, pos);
            std::cout<< "INT4: " << v << '\n';
        }
    }
}
