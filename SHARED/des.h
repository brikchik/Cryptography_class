#pragma comment (lib, "../third-party/lib/Libcrypto_32MT")
#include "../SHARED/Foper.cpp"
#include "../THIRD-PARTY/INCLUDE/openssl/des.h"
#include <Windows.h>
#define cout std::cout
#define cin std::cin
class des_class {
private:
    unsigned char input[8], output[8];
    DES_key_schedule keyschedule;
    DES_key_schedule keyschedule2;
    DES_key_schedule keyschedule3;
    Foper inputfile;
    Foper destfile;
    int paddingNumber;
    std::vector<unsigned char> text;
    std::vector<unsigned char> _EncData;
    std::vector<unsigned char> _DecData;
    DES_key_schedule* getKeySchedule() { return &keyschedule; }
public:
    des_class() {}
    bool set_key(unsigned char* new_key)
    {
        unsigned char temp[8];
        for(int i=0;i<8;i++)temp[i] = new_key[i];
        DES_cblock* dcb = &temp;
        DES_set_key(dcb, getKeySchedule());
        DES_set_key((DES_cblock *)new_key, &keyschedule);
        return true;
    }
    void gen_key() {
        DES_cblock key;
        DES_random_key(&key);
        if(set_key(key))cout << key + '\0';
        DES_set_key((DES_cblock *)key, &keyschedule);
    }
    void gen_key3() {
        DES_cblock key;
        DES_random_key(&key);
        if (set_key(key))cout << key + '\0_';
        DES_set_key((DES_cblock *)key, &keyschedule);
        DES_random_key(&key);
        if (set_key(key))cout << key + '\0_';
        DES_set_key((DES_cblock *)key, &keyschedule2);
        DES_random_key(&key);
        if (set_key(key))cout << key + '\0_';
        DES_set_key((DES_cblock *)key, &keyschedule3);
    }
    void create_key()
    {
        DES_cblock key;
        DES_random_key(&key);
        Foper keyFile;
        for (int i = 0; i < 8; i++)keyFile.GetData().push_back(key[i]);
        keyFile.write(string("key.txt"));
        set_key(key);
        cout <<'[ '<< key+'\0'<<' ]';
    }
    void vector_ecb_encode(std::vector<unsigned char> data, std::vector<unsigned char> &result) {
        result.clear();
        paddingNumber = 0;
        while (((data.size() + paddingNumber) % 8) != 0)
        {
            paddingNumber++;
        }
        for (int i = 0; i < paddingNumber; i++)
        {
            data.push_back(paddingNumber);
        }
        int length = data.size() - 1;
        int pos = 0;
        for (int i = 0; i < length; i += 8) {
            for (int j = 0; j < 8; j++) {
                input[j] = data[i + j];
            }
            DES_ecb_encrypt((DES_cblock *)input, (DES_cblock *)output, &keyschedule, DES_ENCRYPT);
            for (int j = 0; j < 8; j++) {
                result.push_back(output[j]);
                pos++;
            }
        }
        data.clear();
    }
    void vector_ecb_decode(std::vector<unsigned char> data, std::vector<unsigned char> &result) {
        result.clear();
        int length = data.size() - 1;
        int pos = 0;
        for (int i = 0; i < length; i += 8) {
            for (int j = 0; j < 8; j++) {
                input[j] = data[i + j];
            }
            DES_ecb_encrypt((DES_cblock *)input, (DES_cblock *)output, &keyschedule, DES_DECRYPT);
            for (int j = 0; j < 8; j++) {
                result.push_back(output[j]);
                pos++;
            }
        }
        paddingNumber = result.back();
        if (paddingNumber < length) {
            if (result.at(result.size() - paddingNumber) == paddingNumber)
                result._Pop_back_n(paddingNumber);
        }
        data.clear();
    }
    bool ecb_encode(string path, string dest) {
        if (!inputfile.open(path))return false;
        destfile.GetData().clear();
        vector_ecb_encode(inputfile.GetData(), destfile.GetData());
        if (!destfile.write(dest))return false;
        text.clear();
        return true;
    }
    bool ecb_decode(string path, string dest) {
        if (!inputfile.open(path))return false;
        destfile.GetData().clear();
        vector_ecb_decode(inputfile.GetData(), destfile.GetData());
        if (!destfile.write(dest))return false;
        text.clear();
        return true;
    }
    bool des3_ecb_encode(string path, string dest) {
        if (!inputfile.open(path))return false;
        text = inputfile.GetData();
        if (text.empty())return false;
        paddingNumber = 0; //PKCS#7 padding
        while (((text.size() + paddingNumber) % 8) != 0)
        {
            paddingNumber++;
        }
        for (int i = 0; i < paddingNumber; i++)
        {
            text.push_back(paddingNumber);
        }
        std::vector<unsigned char> result;
        int length = text.size() - 1;
        int pos = 0;
        for (int i = 0; i < length; i += 8) {
            for (int j = 0; j < 8; j++) {
                input[j] = text[i + j];
            }
            DES_ecb3_encrypt((DES_cblock *)input, (DES_cblock *)output, &keyschedule, &keyschedule2, &keyschedule3, DES_ENCRYPT);
            for (int j = 0; j < 8; j++) {
                result.push_back(output[j]);
                pos++;
            }
        }
        destfile.GetData() = result;
        if (!destfile.write(dest))return false;
        text.clear();
        return true;
    }
    bool des3_ecb_decode(string path, string dest) {
        if (!inputfile.open(path))return false;
        text = inputfile.GetData();
        if (text.empty())return false;
        std::vector<unsigned char> result;
        int length = text.size() - 1;
        int pos = 0;
        for (int i = 0; i < length; i += 8) {
            for (int j = 0; j < 8; j++) {
                input[j] = text[i + j];
            }
            DES_ecb3_encrypt((DES_cblock *)input, (DES_cblock *)output, &keyschedule,&keyschedule2,&keyschedule3, DES_DECRYPT);
            for (int j = 0; j < 8; j++) {
                result.push_back(output[j]);
                pos++;
            }
        }
        paddingNumber = result.back();
        if (paddingNumber < length) {
            if (result.at(result.size() - paddingNumber) == paddingNumber)
                result._Pop_back_n(paddingNumber);
        }
        destfile.GetData() = result;
        if (!destfile.write(dest))return false;
        text.clear();
        return true;
    }
};