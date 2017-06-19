#include <iostream>
#include <vector>
#include <string>
#include "util.cpp"
#include <locale>
#include <time.h>
#include <map>
#define cout std::cout
#define cin std::cin
util utils;
struct KEY
{
    unsigned char key[8];
    unsigned char key2[8];
    byte bigKey[14];
    void setKey(unsigned char temp[16]) { //u can setkey(any buffer[])
        for (int i = 1; i <= 7; i++) {
            key[i] = temp[i];
            key2[i] = temp[i + 7];
        }
        key[0] = 0; key2[0] = 0;
    };
    void init(unsigned long long x1, unsigned long long x2) //by two 8_byte numbers
    {
        for (int i = 0; i < 6; i++)bigKey[i] = (x1 >> ((5 - i) * 8) & 0xFF);
        for (int i = 0; i < 8; i++)bigKey[i + 6] = (x2 >> ((7 - i) * 8) & 0xFF);
        setKey(bigKey);
    }
};
void doEncrypt()
{
    KEY deskey;
    int a = 65536;//min 2^16
    int b = 262144;//max 2^18
    srand(time(0));
    unsigned long long x2 = 65538;//rand() % b + a;//from left to right. 6bytes from x1, 8 bytes from x2
    deskey.init(0,x2);
    utils._des2(deskey.key, deskey.key2);
}
bool compare(byte* a, byte* b) {
    int k = 0;
    for (int i = 0; i < 24; i++)
        if (a[i] == b[i])k++;
    if (k==24)return true; else return false;
}
void doBuild()
{
    std::map<unsigned int, byte*> table1;
    std::map<unsigned int, byte*> table2;
    int a = 65536;
    int b = 262144;
    utils.restoreDefaults();
    KEY deskey;
    cout << utils._2desEncrypted.data();
    for (int i = 0; i < 1; i++) {
        for (int j = a; j < a+250; j++)
        {
            deskey.init(i, j);
            //utils._data = std::vector<byte>(utils._inputfile.GetData());
            //utils._des(deskey.key);
            //table1.emplace(i, utils._result.data());
            //cout << utils._result.data();
            cin.get();
            utils._data = std::vector<byte>(utils._2desEncrypted);
            utils._undes(deskey.key2);
            byte* importantval;
            for(int i=0;i<utils._result.size();i++)importantval[i] = utils._result.at(i);
            table2.emplace(std::make_pair(j, importantval));
            cout << utils._result.data();
            
            if (compare(utils._importantval, importantval))cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!";
        }
        cout << (long long)table1.max_size();


    }

}

int main()
{
    setlocale(LC_ALL, "RUSSIAN");
    utils.doPrepare();
    doEncrypt();
    doBuild();
    bool k;
    cin.get();
    return 0;
}