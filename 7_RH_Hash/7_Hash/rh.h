#pragma once
#include <vector>
#include "../../Foper.cpp"
std::vector<unsigned char> funcIt(std::vector<unsigned char> iv, std::vector<unsigned char> block);
std::vector<unsigned char> Hash(std::vector<unsigned char> file);
void setBlockLength(int length);
void setOutputSize(int size);