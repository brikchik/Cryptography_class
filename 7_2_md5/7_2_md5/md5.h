#pragma once
#include "../../Foper.cpp"
#include <openssl\md5.h>
#define blockLength 120
class md5class {
public:
    std::vector<unsigned char> Hash(std::vector<unsigned char> file);
};