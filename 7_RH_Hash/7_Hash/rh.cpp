#include "rh.h"
int blockLength = 320;
int outputSize = 32;
std::vector<unsigned char> funcIt(std::vector<unsigned char> iv, std::vector<unsigned char> block)//transforms blockLength bytes into blockLength/10 bytes 
{
    std::vector<unsigned char> res;
    for (int i = 0; i < outputSize; i++)
    {
        unsigned char symbol =
            (block.at(i * 10) + block.at(i * 10 + 2) + block.at(i * 10 + 4) + block.at(i * 10 + 6) + block.at(i * 10 + 8))
            ^
            (block.at(i * 10 + 1) + block.at(i * 10 + 3) + block.at(i * 10 + 5) + block.at(i * 10 + 7) + block.at(i * 10 + 9));
        symbol ^= iv.at(i);
        symbol += i;
        res.push_back(symbol);
    }
    return res;
}
std::vector<unsigned char> Hash(std::vector<unsigned char> file) {
    std::vector<unsigned char> result;
    const std::vector<unsigned char> iv = { 'a','m','c','d','e','f','a','b','l','d','e','f','a','b','c','d','k','f','a','n','c','d','e','f' };
    int size = file.size();
    int maxSize = size;
    while (maxSize % blockLength != 0)maxSize++;
    std::vector<unsigned char> block;
    for (int j = 0; j < maxSize / blockLength; j++)
    {
        for (int i = 0; i < blockLength; i++)
        {
            if ((j * blockLength + i) < size)block.push_back(file.at(j * blockLength + i));
            else block.push_back(0);
        }
        iv = funcIt(iv, block);
        block.clear();
        for (int i = 0; (i < blockLength / 10) && (i < size / blockLength); i++)
        {
            result.push_back(iv.at(i));
        }
    }
    return result;
}

void setBlockLength(int length)
{
    blockLength = length;
}

void setOutputSize(int size)
{
    outputSize = size;
}
