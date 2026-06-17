#pragma once
#include "utils.h"
#include <string>
#include <cstdint>
#include <vector>
#include <fstream>

constexpr uint8_t BINUNICODE= 0x58;
constexpr uint8_t BININT1  = 0x4B;
constexpr uint8_t BININT2 = 0x4D;
constexpr uint8_t BININT= 0x4A;
constexpr uint8_t TUPLE2= 0x86;
constexpr uint8_t TUPLE1= 0x85;
constexpr uint8_t PROTO = 0x80;
constexpr uint8_t MARK = 0x28;
constexpr uint8_t GLOBAL = 0x63;
constexpr uint8_t EMPTY_TUPLE = 0x29;
constexpr uint8_t EMPTY_DICT = 0x7D;
constexpr uint8_t EMPTY_LIST = 0x5D;
constexpr uint8_t BINPUT = 0x71;
constexpr uint8_t LONG_BINPUT = 0x72;
constexpr uint8_t BINGET = 0x68;
constexpr uint8_t LONG_BINGET = 0x6A;
constexpr uint8_t REDUCE = 0x52;
constexpr uint8_t BUILD = 0x62;
constexpr uint8_t TUPLE3 = 0x87;
constexpr uint8_t SETITEM = 0x73;
constexpr uint8_t SETITEMS = 0x75;
constexpr uint8_t APPENDS = 0x61;
constexpr uint8_t NONE = 0x4E;
constexpr uint8_t NEWTRUE = 0x88;
constexpr uint8_t NEWFALSE = 0x89;
constexpr uint8_t STOP = 0x2E;

struct EOCD{
    uint16_t totalEntry;
    uint32_t centralDirOffset;
    uint32_t signature;
    uint16_t diskNumber;
    uint16_t centralDirDisk;
    uint16_t entryThisDisk;
    uint32_t centralDirSize;
    uint16_t comlength;
};
struct zfiles{
    uint32_t signature;
    std::string filename;
    uint32_t namesize;
    uint32_t extra_length;
    uint32_t localheaderOffset;
    uint32_t uncompressed_Size;
    uint32_t dataOffset;
};
struct WData{
    std::string name;
    int storageid;
    std::string dtype;
    std::vector<int64_t>shape;
    long long numel;
};
struct token{
    enum Type{STRING,INT};
    Type type;
    std::string s;
    int v;
};
class Parser{
private:
    std::string read_binunicode(size_t &pos);
    int read_binint1(size_t&pos);
    int read_binint2(size_t&pos);
    int read_binint(size_t&pos);
    std::ifstream file{nullptr};
    EOCD eocd;
    std::vector<zfiles>zip_files;
    std::vector<uint8_t>bytes;
    std::vector<WData>tdata;
public:
EOCD read_eocd(std::string&);
std::vector<zfiles> parse_central_directory();
void create_metadata(std::string& );
void get_weights();
};

