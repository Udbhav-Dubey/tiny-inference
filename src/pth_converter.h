#pragma once
#include "utils.h"
#include <string>
#include <cstdint>
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
EOCD read_eocd(std::string&);

