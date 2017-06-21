#include "../THIRD-PARTY/INCLUDE/openssl/aes.h"
#include "../SHARED/Foper.cpp"
#pragma comment (lib,"../third-party/lib/Libcrypto_32MT.lib")
class aes_class
{
private:
    byte input[16];
    byte enc_out[16];
    AES_KEY enc_key;
    Foper inputfile;
    Foper destfile;
    std::vector<byte> text;
    int paddingNumber;
public:
    aes_class() {}
    std::vector<byte> keystore;
    byte key[16] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
    bool gen_key(int length)
    {
        if (length < 16 || length>256 || length % 32 != 0)return false;
        keystore.clear();
        for (int i = 0; i < length; i++)
        {
            keystore.push_back(rand() % 255);
            key[i] = keystore.at(i);
        }
        std::cout << "[" << key << "] ";
        return true;
    }
    bool set_key(byte new_key[])
    {
        if (strlen((char*)new_key + '\0') < 16)return false;
        for (int i = 0; i < 16; i++)key[i] = new_key[i];
        return true;
    };
    bool encrypt(string path, string dest) {
        if (!inputfile.open(path))return false;
        text = inputfile.GetData();
        paddingNumber = 0; //PKCS#7 padding
        while (((text.size() + paddingNumber) % 16) != 0)
        {
            paddingNumber++;
        }
        for (int i = 0; i < paddingNumber; i++)
        {
            text.push_back(paddingNumber);
        }
        std::vector<byte> result;
        int length = text.size() - 1;
        int pos = 0;
        AES_set_encrypt_key(key, 128, &enc_key);
        for (int i = 0; i < length; i += 16) {

            for (int j = 0; j < 16; j++) {
                input[j] = text[i + j];
            }
            AES_encrypt(input, enc_out, &enc_key);
            for (int j = 0; j < 16; j++) {
                result.push_back(enc_out[j]);
                pos++;
            }
        }
        destfile.GetData() = result;
        if (!destfile.write(dest))return false;
        return true;
    }
    bool decrypt(string path, string dest) {
        Foper inputfile;
        Foper destfile;
        if (!inputfile.open(path))return false;
        text = inputfile.GetData();
        std::vector<byte> result;
        AES_KEY dec_key;
        int length = text.size() - 1;
        int pos = 0;
        AES_set_decrypt_key(key, 128, &dec_key);
        for (int i = 0; i < length; i += 16) {
            for (int j = 0; j < 16; j++) {
                input[j] = text[i + j];
            }
            AES_decrypt(input, enc_out, &dec_key);
            for (int j = 0; j < 16; j++) {
                result.push_back(enc_out[j]);
                pos++;
            }
        }
        paddingNumber = result.back();
        if (result.at(result.size() - paddingNumber) == paddingNumber)
            result._Pop_back_n(paddingNumber);

        destfile.GetData() = result;
        if (!destfile.write(dest))return false;
        return true;
    }
};