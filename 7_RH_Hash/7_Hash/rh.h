#pragma once
#include <vector>
#include "../../Foper.cpp"
class rh
{
private:
    int blockLength=120;
    int outputSize=12;
    std::vector<unsigned char> funcIt(std::vector<unsigned char> &iv, std::vector<unsigned char> &block);
public:
    std::vector<unsigned char> Hash(std::vector<unsigned char> &file);
    void setBlockLength(int length);
    void setOutputSize(int size);
};