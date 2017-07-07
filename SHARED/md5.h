#pragma once
#include "../shared/foper.cpp"
#include "../THIRD-PARTY/INCLUDE/openssl/md5.h"
#pragma comment (lib,"../third-party/lib/Libcrypto_32MT.lib")
class md5class {
public:
    std::vector<byte> md5class::Hash(std::vector<byte> file, int _length, int _outputSize) {
        int blockLength = 120;
        blockLength = _length;
        std::vector<byte> result;
        int size = file.size();
        int maxSize = size;
        while (maxSize % blockLength != 0)maxSize++;
        std::vector<byte> block;
        MD5_CTX md5handler;
        byte* x = new byte[blockLength];
        byte md5digest[MD5_DIGEST_LENGTH];
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
};