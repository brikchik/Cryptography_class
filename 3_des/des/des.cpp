#include <stdio.h> 
#include <iostream>
#include <openssl/des.h>
#include <vector>
#include <fstream>
#include <string>
//#include <openssl/rand.h>
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
class des_class {
private:
    unsigned char input[8], output[8];
    DES_cblock key;
    DES_key_schedule keysched;
public:
    des_class() {}
    void gen_key() {
        DES_random_key(&key);
        std::cout << key;
        DES_set_key((DES_cblock *)key, &keysched);
    }
    bool ecb_encode(std::string path, std::string dest) {
        Foper inputfile;
        Foper destfile;
        if (!inputfile.open(path))return false;
        std::vector<unsigned char> text = inputfile.GetData();
        if (text.empty())return false;
        int paddingNumber = 0; //PKCS#7 padding
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
            DES_ecb_encrypt((DES_cblock *)input, (DES_cblock *)output, &keysched, DES_ENCRYPT);
            for (int j = 0; j < 8; j++) {
                result.push_back(output[j]);
                pos++;
            }
        }
        destfile.GetData() = result;
        if (!destfile.write(dest))return false;
        return true;
    }
    bool ecb_decode(std::string path, std::string dest) {
        Foper inputfile;
        Foper destfile;
        if (!inputfile.open(path))return false;
        std::vector<unsigned char> text = inputfile.GetData();
        if (text.empty())return false;
        std::vector<unsigned char> result;
        int length = text.size() - 1;
        int pos = 0;
        for (int i = 0; i < length; i += 8) {

            for (int j = 0; j < 8; j++) {
                input[j] = text[i + j];
            }
            DES_ecb_encrypt((DES_cblock *)input, (DES_cblock *)output, &keysched, DES_DECRYPT);
            for (int j = 0; j < 8; j++) {
                result.push_back(output[j]);
                pos++;
            }
        }
        //PKCS#7 padding
        int paddingNumber = result.back();
        if (result.at(result.size() - paddingNumber) == paddingNumber)
            result._Pop_back_n(paddingNumber);

        destfile.GetData() = result;
        if (!destfile.write(dest))return false;
        return true;
    }
    bool set_key(unsigned char new_key[])
    {
        if (strlen((char*)new_key + '\0') != 8)return false;
        DES_set_key((DES_cblock *)new_key, &keysched);
        return true;
    }
};

int main()
{
    des_class des;
    std::cout << "Input command (set_key/ecb_ecn/ecb_dec/etc.): " << std::endl;
    std::string mode = "";
    std::cin >> mode;
    std::cout << "Mode: " << mode << std::endl;
    if (mode == "set_key")
    {
        char key_arr[32];
        std::cin >> key_arr;
        if (!des.set_key((unsigned char*)key_arr))return -2;
        std::cout << "Key set.\nInput mode: ";
        std::cin >> mode;
        std::cout << "Mode: " << mode << std::endl;
    }
    std::string source = "";
    std::cin >> source;
    std::cout << "Source: " << source << std::endl;
    std::string dest = "";
    std::cin >> dest;
    std::cout << "Destination: " << dest << std::endl;
    if (mode == "ecb_enc") {
        des.ecb_encode(source, dest);
    }
    if (mode == "ecb_dec") {
        des.ecb_decode(source, dest);
    }
    system("pause");
    return 0;
}