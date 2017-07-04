#pragma once
#include "../shared/foper.h"
class rh
{
private:
    int blockLength = 120;
    int outputSize = 12;
public:
    std::vector<byte> rh::funcIt(std::vector<byte> iv, std::vector<byte> &block) 
    {
        std::vector<byte> res;
        for (int i = 0; i < outputSize - 1; i++)
        {
            byte symbol = (block.at(i) + block.at(i + 1)) % (128 + i);
            symbol ^= iv.at(i);
            symbol += i;
            symbol %= 128;
            res.push_back(symbol);
        }
        byte symbol = block.at(outputSize - 1);
        symbol ^= iv.back();
        res.push_back(symbol);
        return res;
    }
    std::vector<byte> rh::Hash(std::vector<byte> &file,int blockLength=120,int outputSize=12) {
        std::vector<byte> result;
        std::vector<byte> iv = { 'a','m','c','d','e','f','a','b','l','d','e','f','a','b','c','d','k','f','a','n','c','d','e','f','t','y','d','c','q','e','s','x' };
        int size = file.size();
        int maxSize = size;
        while (maxSize % blockLength != 0)maxSize++;
        std::vector<byte> block;
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