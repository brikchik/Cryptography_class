#include <stdio.h> 
#include <iostream>
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <vector>
#include <fstream>
#include <string>
#include <Windows.h>
class Foper
{
private:
    std::vector<unsigned char> _data;
public:
    Foper()
    {
    }
    bool open(std::string &name)
    {
        std::ifstream _file(name, std::ios::in | std::ios::binary);
        if (!_file.is_open())
        {
            std::cout << "No such file: " << name << std::endl;
            return false;
        }

        _file.seekg(0, std::ios::end);
        int size = _file.tellg();
        _file.seekg(0, std::ios::beg);

        if (size != 0) {
            unsigned char byte;
            for (int i = 0; i < size; i++)
            {
                byte = (unsigned char)(_file.get());
                _data.push_back(byte);
            }
        }
        else {
            std::cout << "Empty file";
            _file.close();
        }
        return !_data.empty();
    }
    std::vector<unsigned char> &GetData() { return _data; }
    bool write(std::string &output)
    {
        std::ofstream outfile(output, std::fstream::out | std::fstream::trunc | std::fstream::binary);
        if (!outfile.is_open()) { std::cout << "Unable to open file for writing"; return false; };
        for (int i = 0; i < _data.size(); i++)
        {
            outfile.put((unsigned char)_data.at(i));
        }
        outfile.close();
        return true;
    }
};
class aes_class
{
private:
    unsigned char input[16];
    unsigned char enc_out[16];
    AES_KEY enc_key;
    Foper inputfile;
    Foper destfile;
    std::vector<unsigned char> text;
    int paddingNumber;
public:
    aes_class() {}
    std::vector<unsigned char> keystore;
    unsigned char key[16] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };
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
    bool set_key(unsigned char new_key[])
    {
        if (strlen((char*)new_key + '\0') < 16)return false;
        for (int i = 0; i < 16; i++)key[i] = new_key[i];
        return true;
    };
    bool encrypt(std::string path, std::string dest) {
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
        std::vector<unsigned char> result;
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
    bool decrypt(std::string path, std::string dest) {
        Foper inputfile;
        Foper destfile;
        if (!inputfile.open(path))return false;
        text = inputfile.GetData();
        std::vector<unsigned char> result;
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
        //PKCS#7 padding
        paddingNumber = result.back();
        if (result.at(result.size() - paddingNumber) == paddingNumber)
            result._Pop_back_n(paddingNumber);

        destfile.GetData() = result;
        if (!destfile.write(dest))return false;
        return true;
    }
};
int main()
{
    aes_class aes;
    system("cls");
    std::cout << "Input command (gen_key/set_key/enc/dec): " << std::endl;
    std::string mode = "";
    std::cin >> mode;
    std::cout << "Mode: " << mode << std::endl;
    if (mode == "set_key")
    {
        char key_arr[32];
        std::cin >> key_arr;
        if (!aes.set_key((unsigned char*)key_arr))return -2;
        std::cout << "Key set.\nInput mode: ";
        std::cin >> mode;
        std::cout << "Mode: " << mode << std::endl;
    }
    else
        if (mode == "gen_key" || mode == "create_key")
        {
            std::cout << "Input key length: ";
            int length;
            std::cin >> length;
            aes.gen_key(length);
            if (mode == "create_key")
            {
                Foper keyfile;
                keyfile.GetData() = aes.keystore;
                keyfile.write(std::string("key.txt"));
            }
            std::cout << "Key set.\nInput mode: ";
            std::cin >> mode;
            std::cout << "Mode: " << mode << std::endl;
        }
    std::string source = "";
    std::cout << "Source: ";
    std::cin >> source;
    std::cout << std::endl;
    std::string dest = "";
    std::cout << "Destination: ";
    std::cin >> dest;
    std::cout << std::endl;
    if (mode == "enc") {
        aes.encrypt(source, dest);
    }
    if (mode == "dec") {
        aes.decrypt(source, dest);
    }
    system("pause");
    return 0;
}