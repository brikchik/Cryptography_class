#pragma once
#include "../../Foper.cpp"
#include <openssl\md5.h>
class md5class {
public:
    std::vector<unsigned char> Hash(std::vector<unsigned char> file,int length,int outputSize);
};