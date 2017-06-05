#include "imports.h"
class rsa
{
    //some functions from www.hayageek.com/rsa-encryption-decryption-openssl-c
private:
    RSA * createRSA(unsigned char * key, int publicKey)
    {
        RSA *rsa = NULL;
        BIO *keybio;
        keybio = BIO_new_mem_buf(key, -1);
        if (keybio == NULL)
        {
            std::cout<<"Failed to create key BIO";
            return 0;
        }
        if (publicKey)
        {
            rsa = PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
        }
        else
        {
            rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL, NULL);
        }
        return rsa;
    }
    RSA * createRSAWithFilename(char * filename, int publicKey)
    {
        Foper fp;
        if (!fp.open(string(filename)))
        {
            std::cout << "Unable to open file " << filename << std::endl;
            return NULL;
        }
        _byte text[4096];
        for (int i = 0; i < fp.GetData().size(); i++)text[i] = fp.GetData().at(i);
        RSA *rsa = RSA_new();
        rsa = createRSA(text, publicKey);
        return rsa;
    }
public:
    int padding = RSA_PKCS1_PADDING;
    int public_encrypt(unsigned char * data, int data_len, unsigned char * key, bool FromFile, unsigned char *encrypted)
    {
        RSA * rsa;
        if (FromFile)
        {
            rsa = createRSAWithFilename((char*)key, 1);
        }
        else
        {
            rsa = createRSA(key, 1);
        }
        int result = RSA_public_encrypt(data_len, data, encrypted, rsa, padding);
        return result;
    }
    int private_decrypt(unsigned char * enc_data, int data_len, unsigned char * key, bool FromFile, unsigned char *decrypted)
    {
        RSA * rsa;
        if (FromFile)
        {
            rsa = createRSAWithFilename((char*)key, 0);
        }
        else
        {
            rsa = createRSA(key, 0);
        }
        int  result = RSA_private_decrypt(data_len, enc_data, decrypted, rsa, padding);
        return result;
    }
    int private_encrypt(unsigned char * data, int data_len, unsigned char * key, bool FromFile, unsigned char *encrypted)
    {
        RSA * rsa;
        if (FromFile)
        {
            rsa = createRSAWithFilename((char*)key, 0);
        }
        else
        {
            rsa = createRSA(key, 0);
        }
        int result = RSA_private_encrypt(data_len, data, encrypted, rsa, padding);
        return result;
    }
    int public_decrypt(unsigned char * enc_data, int data_len, unsigned char * key, bool FromFile, unsigned char *decrypted)
    {
        RSA * rsa;
        if (FromFile)
        {
            rsa = createRSAWithFilename((char*)key, 1);
        }
        else
        {
            rsa = createRSA(key, 1);
        }
        int  result = RSA_public_decrypt(data_len, enc_data, decrypted, rsa, padding);
        return result;
    }
};