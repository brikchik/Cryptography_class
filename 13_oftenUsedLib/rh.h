#pragma once
#include "../shared/Foper.cpp"
class rh
{
private:
    int blockLength = 120;
    int outputSize = 12;
public:
    std::vector<unsigned char> rh::funcIt(std::vector<unsigned char> iv, std::vector<unsigned char> &block) 
    {
        std::vector<unsigned char> res;
        for (int i = 0; i < outputSize - 1; i++)
        {
            unsigned char symbol = (block.at(i) + block.at(i + 1)) % (128 + i);
            symbol ^= iv.at(i);
            symbol += i;
            res.push_back(symbol);
        }
        unsigned char symbol = block.at(outputSize - 1);
        symbol ^= iv.back();
        res.push_back(symbol);
        return res;
    }
    std::vector<unsigned char> rh::Hash(std::vector<unsigned char> &file) {
        std::vector<unsigned char> result;
        std::vector<unsigned char> iv = { 'a','m','c','d','e','f','a','b','l','d','e','f','a','b','c','d','k','f','a','n','c','d','e','f','t','y','d','c','q','e','s','x' };
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
            for (int i = 0; (i < outputSize) && (i < size); i++)
            {
                result.push_back(iv.at(i));
            }
        }
        return result;
    }

    void rh::setBlockLength(int length)
    {
        blockLength = length;
    }

    void rh::setOutputSize(int size)
    {
        outputSize = size;
    }
};