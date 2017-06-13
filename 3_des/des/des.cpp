#include <stdio.h> 
#include <iostream>
#include <openssl/des.h>
#include <vector>
#include <fstream>
#include <string>
//#include <openssl/rand.h>
#include <Windows.h>
#define cout std::cout
#define cin std::cin
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
            cout << "No such file: " << name << std::endl;
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
        else
        {
            cout << "Empty file";
            _file.close();
        }
        return !_data.empty();
    }
    std::vector<unsigned char> &GetData() { return _data; }
    bool write(std::string &output)
    {
        std::ofstream outfile(output, std::fstream::out | std::fstream::trunc | std::fstream::binary);
        if (!outfile.is_open()) { cout << "Unable to open file for writing"; return false; };
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
    DES_key_schedule keyschedule;
    DES_key_schedule keyschedule2;//for 3DES
    DES_key_schedule keyschedule3;//for 3DES
    Foper inputfile;
    Foper destfile;
    int paddingNumber;
    std::vector<unsigned char> text;
    std::vector<unsigned char> _EncData;
    std::vector<unsigned char> _DecData;
public:
    des_class() {}
    bool set_key(unsigned char *new_key)
    {
        if (strlen((char*)new_key + '\0') != 9)return false;
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
        keyFile.write(std::string("key.txt"));
        set_key(key);
        cout <<'[ '<< key+'\0'<<' ]';
    }
    bool ecb_encode(std::string path, std::string dest) {
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
            DES_ecb_encrypt((DES_cblock *)input, (DES_cblock *)output, &keyschedule, DES_ENCRYPT);
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
    bool ecb_decode(std::string path, std::string dest) {
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
            DES_ecb_encrypt((DES_cblock *)input, (DES_cblock *)output, &keyschedule, DES_DECRYPT);
            for (int j = 0; j < 8; j++) {
                result.push_back(output[j]);
                pos++;
            }
        }
        paddingNumber = result.back();//PKCS#7 padding
        if (paddingNumber < length) { //for very small (X<16b) messages
            if (result.at(result.size() - paddingNumber) == paddingNumber)
                result._Pop_back_n(paddingNumber);
        }
        destfile.GetData() = result;
        if (!destfile.write(dest))return false;
        text.clear();
        return true;
    }
    DES_cblock _key;

#define CFB 0
#define OFB 1
#define CBC 2
#define TripleDes true
#define Des false
    void encrypt_ecb(std::vector<unsigned char> &Data, int Mode, bool TripleCheck)
    {
        DES_cblock input;
        DES_cblock output;
        int Size = Data.size();
        int PseudoSize = (Data.size() + 7) / 8 * 8;
        cout << PseudoSize<<" ";
        int k = 0;
        for (int i = 0; i <PseudoSize; i++)
        {
            if (i<Size)
            {
                input[k] = Data.at(i);
            }
            else
            {
                input[k] = NULL;
            }
            k++;
            if (k == 8)
            {
                if (TripleCheck)
                {
                    DES_ecb3_encrypt(&input, &output, &keyschedule, &keyschedule2, &keyschedule3, Mode);
                }
                else
                {
                    DES_ecb_encrypt(&input, &output, &keyschedule, Mode);
                }
                for (int j = 0; j < 8; j++)
                {
                    if (Mode)
                    {
                        _EncData.push_back((unsigned char)output[j]);
                    }
                    else
                    {
                        _DecData.push_back((unsigned char)output[j]);
                    }
                    input[j] = NULL;
                }
                k = 0;
            }
        }
    }
    void EncryptECB(std::string path, std::string dest_path, const char *key)
    {
        inputfile.open(path);
        text = inputfile.GetData();
        //DES_string_to_key(key, &_key);
        //DES_set_key_checked(&_key, &keyschedule);
        encrypt_ecb(text, DES_ENCRYPT, Des);
        destfile.GetData() = _EncData;
        _EncData.clear();
        _DecData.clear();
        destfile.write(dest_path);
        cout << _EncData.size()<<" ";
        cout << _DecData.size()<<" ";
    }
    void DecryptECB(std::string path, std::string dest_path, const char *key)
    {
        inputfile.open(path);
        text = inputfile.GetData();
        //DES_string_to_key(key, &_key);
        //DES_set_key_checked(&_key, &keyschedule);
        encrypt_ecb(text, DES_DECRYPT, Des);
        destfile.GetData() = _DecData;
        destfile.write(dest_path);
        _EncData.clear();
        _DecData.clear();
        cout << _EncData.size() << " ";
        cout << _DecData.size() << " ";
    }
    void EncryptCBC(std::string path, std::string dest_path, const char *key)
    {
        inputfile.open(path);
        text = inputfile.GetData();
        encrypt_cfb_ofb_cbc(text, DES_ENCRYPT, CBC, Des);
        destfile.GetData() = _EncData;
        destfile.write(dest_path);
        _EncData.clear();
        _DecData.clear();
    }
    void DecryptCBC(std::string path, std::string dest_path, const char *key)
    {
        inputfile.open(path);
        text = inputfile.GetData();
        encrypt_cfb_ofb_cbc(text, DES_DECRYPT, CBC, Des);
        destfile.GetData() = _DecData;
        destfile.write(dest_path);
        _EncData.clear();
        _DecData.clear();
    }
    void EncryptCFB(std::vector<unsigned char> &Data, const char *key)
    {
        DES_string_to_key(key, &_key);
        DES_set_key_checked(&_key, &keyschedule);
        encrypt_cfb_ofb_cbc(Data, DES_ENCRYPT, CFB, Des);
    }
    void DecryptCFB(std::vector<unsigned char> &Data, const char *key)
    {
        DES_string_to_key(key, &_key);
        DES_set_key_checked(&_key, &keyschedule);
        encrypt_cfb_ofb_cbc(Data, DES_DECRYPT, CFB, Des);
    }
    void EncryptOFB(std::vector<unsigned char> &Data, const char *key)
    {
        DES_string_to_key(key, &_key);
        DES_set_key_checked(&_key, &keyschedule);
        encrypt_cfb_ofb_cbc(Data, DES_ENCRYPT, OFB, Des);
    }
    void DecryptOFB(std::vector<unsigned char> &Data, const char *key)
    {
        DES_string_to_key(key, &_key);
        DES_set_key_checked(&_key, &keyschedule);
        encrypt_cfb_ofb_cbc(Data, DES_DECRYPT, OFB, Des);
    }
    void encrypt_cfb_ofb_cbc(std::vector<unsigned char> &Data, int Mode, int CFBorOFBorCBC, bool TripleCheck)
    {
        DES_cblock IV = { 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33 };
        unsigned int Size = Data.size();
        unsigned int PseudoSize = (Size + 7) / 8 * 8;
        unsigned char *Input = new unsigned char[Size];
        unsigned char *Output = new unsigned char[Size];
        unsigned char *InputforCBC = new unsigned char[PseudoSize];
        unsigned char *OutputforCBC = new unsigned char[PseudoSize];
        int num = 0;
        if (CFBorOFBorCBC != CBC)
        {
            for (unsigned int i = 0; i < Size; i++)
            {
                Input[i] = Data.at(i);
            }
        }
        else
        {
            for (unsigned int i = 0; i < Size; i++)
            {
                if (i < Size)
                {
                    InputforCBC[i] = Data.at(i);
                }
                else
                {
                    InputforCBC[i] = NULL;
                }
            }
        }
        if (CFBorOFBorCBC == 0)
        {
            if (TripleCheck)
            {
                DES_ede3_cfb64_encrypt(Input, Output, Size, &keyschedule, &keyschedule2, &keyschedule3, &IV, &num, Mode);
            }
            else
            {
                DES_cfb_encrypt(Input, Output, 8, Size, &keyschedule, &IV, Mode);
            }
        }
        else if (CFBorOFBorCBC == 1)
        {
            if (TripleCheck)
            {
                DES_ede3_ofb64_encrypt(Input, Output, Size, &keyschedule, &keyschedule2, &keyschedule3, &IV, &num);
            }
            else
            {
                DES_ofb_encrypt(Input, Output, 8, Size, &keyschedule, &IV);
            }
        }
        else
        {
            if (TripleCheck)
            {
                DES_ede3_cbc_encrypt(InputforCBC, OutputforCBC, Size, &keyschedule, &keyschedule2, &keyschedule3, &IV, Mode);
            }
            else
            {
                DES_ncbc_encrypt(InputforCBC, OutputforCBC, Size, &keyschedule, &IV, Mode);
            }
        }
        if (CFBorOFBorCBC != CBC)
        {
            for (unsigned int i = 0; i < Size; i++)
            {
                if (Mode)
                {
                    _EncData.push_back(Output[i]);
                }
                else
                {
                    _DecData.push_back(Output[i]);
                }
            }
        }
        else
        {
            for (unsigned int i = 0; i < PseudoSize; i++)
            {
                if (Mode)
                {
                    _EncData.push_back(OutputforCBC[i]);
                }
                else
                {
                    _DecData.push_back(OutputforCBC[i]);
                }
            }
        }
        delete Input;
        delete Output;
    }
    bool cbc_encode(std::string path, std::string dest) {
        if (!inputfile.open(path))return false;
        text = inputfile.GetData();
        if (text.empty())return false;
        paddingNumber = 0; //PKCS#7 padding
        while (((text.size() + paddingNumber) % 8) != 0)paddingNumber++;
        for (int i = 0; i < paddingNumber; i++)text.push_back(paddingNumber);
        std::vector<unsigned char> result;
        unsigned int length = text.size() - 1;
        int Size = (text.size() + 7) / 8 * 8;
        unsigned char* data = new unsigned char[length];
        unsigned char* res = new unsigned char[length];
        unsigned char *InputforCBC = new unsigned char[length];
        unsigned char *OutputforCBC = new unsigned char[length];
        for (unsigned int i = 0; i < Size; i++)
        {
            if (i < length)
            {
                InputforCBC[i] = text.at(i);
            }
            else
            {
                InputforCBC[i] = NULL;
            }
        }
        int pos = 0;
        DES_cblock ivec = {'v','e','c','t','o','r','k','e'};
                DES_ncbc_encrypt(InputforCBC, OutputforCBC,Size, &keyschedule, &ivec,DES_ENCRYPT);
                result.clear();
        for (int i = 0; i < Size; i++)result.push_back(OutputforCBC[i]);
        destfile.GetData() = result;
        if (!destfile.write(dest))return false;
        return true;
    }
    bool cbc_decode(std::string path, std::string dest) {
        if (!inputfile.open(path))return false;
        text = inputfile.GetData();
        if (text.empty())return false;
        std::vector<unsigned char> result;
        unsigned int length = text.size() - 1;
        unsigned char* data = new unsigned char[length];
        unsigned char* res = new unsigned char[length];
        unsigned char *InputforCBC = new unsigned char[length];
        unsigned char *OutputforCBC = new unsigned char[length];
        int Size = text.size();
        DES_cblock ivec = { 'v','e','c','t','o','r','k','e' };
        for (unsigned int i = 0; i < Size; i++)
        {
            if (i < length)
            {
                InputforCBC[i] = text.at(i);
            }
            else
            {
                InputforCBC[i] = NULL;
            }
        }
        DES_ncbc_encrypt(InputforCBC, OutputforCBC, Size, &keyschedule, &ivec, DES_DECRYPT);
        result.clear();
        for (int i = 0; i < Size; i++)result.push_back(OutputforCBC[i]);
        destfile.GetData() = result;
        if (!destfile.write(dest))return false;
        return true;
    }
    bool des3_ecb_encode(std::string path, std::string dest) {
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
    bool des3_ecb_decode(std::string path, std::string dest) {
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
        paddingNumber = result.back();//PKCS#7 padding
        if (paddingNumber < length) { //for very small (X<16b) messages
            if (result.at(result.size() - paddingNumber) == paddingNumber)
                result._Pop_back_n(paddingNumber);
        }
        destfile.GetData() = result;
        if (!destfile.write(dest))return false;
        text.clear();
        return true;
    }
};

int main()
{
    des_class des;
    //des.gen_key3();
    //des.cbc_encode("input.docx","out");
    //des.cbc_decode("out", "result.docx");
    /*des.gen_key();
    des.EncryptCBC("input.txt", "result.txt", "helloew");
    des.DecryptCBC("result.txt", "output.docx", "helloew");*/
    /*unsigned char* val = (unsigned char*)"helloworld";
    std::vector<unsigned char> Data;
    for (int i = 0; i < des._EncData.size(); i++)Data.push_back(val[i]);
    des.EncryptCBC(Data, "helloke");
    for (int i = 0; i < Data.size(); i++)cout << Data.at(i);
    cout << std::endl;
    for (int i = 0; i < des._EncData.size(); i++)cout << des._EncData.at(i);*/
    system("cls");
    cout << "Input command (gen_key/set_key/create_key/ecb_enc/ecb_dec): " << std::endl;
    std::string mode = "";
    cin >> mode;
    cout << "Mode: " << mode << std::endl;
    if (mode == "set_key")
    {
        char key_arr[32];
        cin >> key_arr;
        if (!des.set_key((unsigned char*)key_arr))return -2;
        cout << "Key set.\nInput mode: ";
        cin >> mode;
        cout << "Mode: " << mode << std::endl;
    }
    else
        if (mode == "gen_key" || mode=="create_key")
        {
            des.gen_key();
            if (mode == "create_key")des.create_key();
            cout << "Key set.\nInput mode: ";
            cin >> mode;
            cout << "Mode: " << mode << std::endl;
        }
    std::string source = "";
    cout << "Source: ";
    cin >> source;
    cout << std::endl;
    std::string dest = "";
    cout << "Destination: ";
    cin >> dest;
    cout << std::endl;
    if (mode == "ecb_enc") {
        des.ecb_encode(source, dest);
    }
    else
    if (mode == "ecb_dec") {
        des.ecb_decode(source, dest);
    }
    else
    if (mode == "cbc_enc") {
        //des.cbc_encode(source, dest);
    }
    else
    if (mode == "ecb_dec") {
        //des.ecb_decode(source, dest);
    }
    system("pause");
    return 0;
}