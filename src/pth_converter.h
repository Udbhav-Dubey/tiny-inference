#pragma once
#include "utils.h"
#include <string>
#include <cstdint>
#include <vector>
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
    uint32_t localheaderOffset;
    uint32_t uncompressed_Size;
};
class Parser{
private:
    std::ifstream file{nullptr};
    EOCD eocd;
public:
EOCD read_eocd(std::string&);
std::vector<zfiles> parse_central_directory();
};
