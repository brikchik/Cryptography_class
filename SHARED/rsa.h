#pragma once
#pragma comment (lib,"../third-party/lib/libcrypto_32MT.lib")
#include "../shared/Foper.cpp"
#include "../THIRD-PARTY/INCLUDE/openssl/rsa.h"
#include "../THIRD-PARTY/INCLUDE/openssl/pem.h"
#include "../THIRD-PARTY/INCLUDE/openssl/bio.h"
class RSAclass
{
private:
    int padding = RSA_PKCS1_PADDING;
    RSA* createRSA(byte * key, int publicKey)
    {
        RSA *rsa = NULL;
        BIO *keybio = BIO_new_mem_buf(key, -1);
        if (keybio == NULL)
        {
            std::cout << "Failed to create key BIO";
            return 0;
        }
        if (publicKey)
        {
            rsa = PEM_read_bio_RSAPublicKey(keybio, &rsa, NULL, NULL);
        }
        else
        {
            rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
        }
        return rsa;
    }
    RSA * createRSAFromFile(char * filename, int publicKey)
    {
        Foper fp;
        if (!fp.open(string(filename)))
        {
            std::cout << "Unable to open file " << filename << std::endl;
            return NULL;
        }
        byte text[2048];
        for (int i = 0; i < fp.GetData().size(); i++)text[i] = fp.GetData().at(i);
        RSA *rsa = RSA_new();
        rsa = createRSA(text, publicKey);
        return rsa;
    }
    std::vector<byte> result;
public:
    int Encrypt(string sourceFile, string destFile, char* key)
    {
        RSA* rsa = createRSAFromFile(key, 1);
        if (!rsa) { std::cout << "key not found"; system("pause"); return -1; }
        Foper inputFile;
        if (!inputFile.open(sourceFile))return -1;
        int blockSize = RSA_size(rsa) - 11;
        byte* temp = new byte[blockSize];
        byte* crypted = new byte[blockSize];
        int size = inputFile.GetData().size();
        for (int i = 0; i < size; i+=blockSize)
        {
            for (int j = 0; j < blockSize; j++)
            {
                if ((j + i) < size) temp[j] = inputFile.GetData().at(j + i);
            }
            if (size < blockSize+i) blockSize = size - i;
            int bytesEncrypted = RSA_public_encrypt(blockSize, temp, crypted, rsa, padding);
            for (int j = 0; j < bytesEncrypted; j++)
            {
                result.push_back(crypted[j]);
            }
        }
        Foper outputFile;
        outputFile.GetData() = result;
        outputFile.write(destFile);
        result.clear();
        return 0;
    }
    int Decrypt(string sourceFile, string destFile, char* key)
    {
        RSA* rsa = createRSAFromFile(key, 0);
        if (!rsa) { std::cout << "key not found"; system("pause"); return -1; }
        Foper inputFile;
        if (!inputFile.open(sourceFile))return -1;
        int blockSize = RSA_size(rsa);
        byte* temp = new byte[blockSize];
        byte* crypted = new byte[blockSize];
        int fullSize = inputFile.GetData().size();
        
        for (int i = 0; i < fullSize; i+=blockSize)
        {
            for (int j = 0; j < blockSize; j++)
            {
                temp[j] = inputFile.GetData().at(j + i);
            }
            int bytesDecrypted = RSA_private_decrypt(blockSize, temp, crypted, rsa, padding);
            for (int j = 0; j < bytesDecrypted; j++)
            {
                result.push_back(crypted[j]);
            }
            Foper outputFile;
            outputFile.GetData() = result;
            outputFile.write(destFile);
        }
        result.clear();
        return 0;
    }
};