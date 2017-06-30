#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "util.cpp"
#include <locale>
#include <time.h>
#include <map>
#pragma comment(linker, "/STACK:65536")
#define cout std::cout
#define cin std::cin
util utils;
struct KEY
{
    unsigned char key[8];
    unsigned char key2[8];
    //unsigned char bigKey[14];
    //void setKey(unsigned char temp[14]) { //u can setkey(any buffer[])
    //    for (int i = 0; i < 7; i++) {
    //        key[i] = temp[i];
    //        key2[i] = temp[i + 7];
    //    }
    //    key[8] = 0; key2[8] = 0;
    //};
    void init(unsigned long long x1, unsigned long long x2) //by two 8_byte numbers
    {
        for (int i = 0; i < 8; i++)key[i] = (x1 >> ((7 - i ) * 8) & 0xFF);
        for (int i = 0; i < 8; i++)key2[i] = (x2 >> ((7 - i) * 8) & 0xFF);
        key[7] = 0;
        key2[7] = 0;
    }
};
void doEncrypt(std::vector<unsigned char> ax, std::vector<unsigned char> bx)
{
    KEY deskey;
    int a = 65536;//min 2^16
    int b = 262144;//max 2^18
    srand(time(0));
    unsigned long long x2 = 65543;//rand() % b + a;//from left to right. 6bytes from x1, 8 bytes from x2
    deskey.init(0,x2);
    utils._des2(deskey.key, deskey.key2,ax,bx);
}
bool compare(std::vector<unsigned char> a, std::vector<unsigned char> b) {
    bool k = true;
    for (int i = 0; i < 20; i++)
        if (a.at(i) != b.at(i))k = false;
    if (k)return true; else return false;
}
void doBuild(std::vector<unsigned char> data, std::vector<unsigned char> result)
{
    std::ofstream d("log.txt");
    std::vector<std::vector<unsigned char>> vec2;
    int a = 65536;
    int b = 262144;
    KEY deskey;
    int e=0;
    data = utils._des2(deskey.key, deskey.key2, data, result);
    std::vector<unsigned char> vectR=std::vector<unsigned char>(data);
    for (unsigned long long i = 0; i < 1; i++) {
        for (unsigned long long j = a; j < b; j++)
        {
            try{
            //cout << j-a << '_';
                deskey.init(i, j);
                //utils._data = std::vector<unsigned char>(utils._inputfile.GetData());
                //utils._des(deskey.key);
                //table1.emplace(i, utils._result.data());
                //cin.get();
                utils._undes(deskey.key2, data, result);
                vec2.push_back(result);
                d << result.data();
                //table2.emplace(std::make_pair(j, importantval));
                //cout << utils._result.data();
                if (compare(vectR, result))cout<<"\nSUCCESS\n";
            }
            catch (...) {}//e++; cout << j - a<<'_'; }
            //cout << e << '_'<<'|'; }            
        }
        //cout << e;
        cout << vec2.size()<<'/'<<b-a;
        d.close();
        //cout << (long long)table1.max_size();


    }

}

int main()
{
    setlocale(LC_ALL, "RUSSIAN");
    std::vector<unsigned char> data;
    std::vector<unsigned char> result;
    data=utils.doPrepare();
    doEncrypt(data,result);
    doBuild(data, result);
    cin.get();
    return 0;
}