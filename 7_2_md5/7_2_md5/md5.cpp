#include "md5.h"
std::vector<unsigned char> md5class::Hash(std::vector<unsigned char> file, int _length,int _outputSize) {
    int blockLength = 120;
	    blockLength=_length;
        std::vector<unsigned char> result;
        int size = file.size();
        int maxSize = size;
        while (maxSize % blockLength != 0)maxSize++;
        std::vector<unsigned char> block;
        MD5_CTX md5handler;
        unsigned char* x=new unsigned char[blockLength];
        unsigned char md5digest[MD5_DIGEST_LENGTH];
        for (int i = 0; i < maxSize / blockLength; i++)
        {
            for (int j = 0; j < blockLength; j++)
                if (j + blockLength*i < size)x[j] = file.at(j + blockLength * i); else x[j] = 0;
            MD5_Init(&md5handler);
            MD5_Update(&md5handler, x, blockLength);
            MD5_Final(md5digest, &md5handler);
            for (int i = 0; i < _outputSize; i++)result.push_back(md5digest[i]);//taking first bytes of md5 hash
        }
        return result;
    }
