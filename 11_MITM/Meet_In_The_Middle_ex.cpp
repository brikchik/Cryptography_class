#include <iostream>
#include <vector>
#include <string>
#include "util.cpp"
#include <locale>
#include <time.h>
#include <map>
#include <fstream>
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
    unsigned long long x2 = 65540;//rand() % b + a;//from left to right. 6bytes from x1, 8 bytes from x2
    deskey.init(0, x2);
    utils._des2(deskey.key, deskey.key2);
}
bool compare(string a, string b) {
    int k = 0;
    for (int i = 0; i < 24; i++)
        if (a[i] == b[i])k++;
    if (k == 24)return true; else return false;
}
std::vector<string> vect;
void doBuild()
{
    std::map<unsigned int, byte*> table1;
    std::map<unsigned int, byte*> table2;
    std::fstream fo("log.txt");
    int a = 65536;
    int b = 262144;
    utils.restoreDefaults();
    KEY deskey;
    int x = 0;
    cout << utils._2desEncrypted.data();
    for (int i = 0; i < 1; i++) {
        for (int j = a; j < b; j++)
        {
            try {
                deskey.init(i, j);
                //cin.get();
                utils._data = std::vector<byte>(utils._2desEncrypted);
                utils._undes(deskey.key2);
                byte importantval[24];// = new byte[24];
                for (int i = 0; i < utils._result.size(); i++)importantval[i] = utils._result.at(i);
                vect.push_back((string)((char*)importantval));
                fo << importantval<<'\0';
                x++;
            }
            catch (...) {};
        }
        cout << x;
    }
    fo.close();
}

int main()
{
    setlocale(LC_ALL, "en_US.UTF-8");
    utils.doPrepare();
    doEncrypt();
    doBuild();
    KEY deskey;
    utils.doPrepare(); vect.begin();
    deskey.init(0, 66000);
    utils._des(deskey.key);
    for (int i = 0; i < utils._result.size(); i++) {
        
            if (vect[i] == string((char*)utils._result.data()))cout << "fgrhtjyujyhtgrfergthyjuyhtgfdgthyju";
            else cout << string((char*)utils._result.data());
            //if (compare(vect.at(i), string((char*)utils._result.data())))
            //    exit(4);
    }
    cout << 5;
    cin.get();
    return 0;
}