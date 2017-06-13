#include <map>
#include <stdint.h>
#include <iostream>
#include <vector>
#define map std::map
//vector<_byte,_byte> mas;
//int mas[256];
//int mas2[65536];
#include <stdint.h>
int b() {
    FILE *inputfile;
    fopen_s(&inputfile, "file.txt", "rb");
    if (!inputfile)return 1;
    fseek(inputfile, 0, SEEK_END);
    int size = ftell(inputfile);
    fseek(inputfile, 0, SEEK_SET);
    unsigned char* x = new unsigned char[size];
    /*char* y = new char[size];*/
    for (int i = 0; i < size; i++)x[i] = 0;
    fread(x, sizeof(unsigned char), size, inputfile);
    fseek(inputfile, 0, SEEK_SET);
    double pi = 0;
    double P = 0;
    map<char, int> map1;
    for (int i = 0; i < size; i++)
    {
        if (map1.find(x[i]) == map1.end()) map1.insert(std::pair<char, int>(x[i], 1));
        else
            map1.at(x[i])++;
    }

    for (auto it = map1.begin(); it != map1.end(); ++it)
    {
        pi = 1.0*it->second / size;
        P -= log(pi) / log(8.0)*pi;
    }
    std::cout << P << ' ';
    return 0;
}
int main()
{
    b();
    //fread(y, sizeof(char), size, inputfile);*/
    //for (int i = 0; i < size; i++)std::cout << (int)x[i] << ' ';
    //std::cout << '\n';
    //for (int i = 0; i < size; i++)
    //{
    //    mas[x[i]]++;
    //    mas2[y[i]]++;
    //}
    //double pi=0;
    //double P=0;
    //double P2 = 0;
    //double P3 = 0;
    //int count=0;//alphabet
    //for (int i = 0; i < 256; i++)if (mas[i])count++;
    //std::cout << count;
    //for (int i = 0; i < 256; i++)
    //{
    //    if (mas[i] == 0)pi = 0;
    //    else
    //    {
    //        pi = (double)(1.0*mas[x[i]] / count);
    //        std::cout << (int)mas[x[i]]<<' ';
    //        P3 +=pi;
    //        //std::cout << pi << ' ';
    //        P += -log(pi) / log(8)*pi;
    //    }
    //}
    //for (int i = 0; i < 65536; i++)
    //{
    //    if (mas2[i] == 0)pi = 0;
    //    else
    //    {
    //        pi = (double)(1.0*(int)mas2[x[i]] / size);
    //        std::cout << pi << ' ';
    //        //P3 += pi;
    //        P2 += -log(pi)/log(16)*pi;
    //    }
    //}

    //std::cout << P2 << std::endl;

    //std::cout << P3 << std::endl;
    system("pause");
    return 0;
}

