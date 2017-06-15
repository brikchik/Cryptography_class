
#include <Windows.h>
#include <iostream>
#include <string>
#include <iomanip>
#include <stdio.h>
#include <conio.h>
#include <tchar.h>
#include <vector>
#include "../../7_Hash/7_Hash/rh.h"
#include "hash.cpp"
int main()
{
    std::cin.clear();
    /*wchar_t* filename= (wchar_t*)"C:/Users/ֳכוב/Documents/GitHub/Cryptography_class/3_DES/des/des.cpp";
    std::cout << filename;
    bool done=true;
    do
    {
        std::cout << "Type in the path to the file" << std::endl;
        if (!fgetws(filename, 256, stdout)) {
            done = true;
        }
        else done = true;
    } while (!done);*/
    HashMap hm;
    hm.put(13,"str1");
    hm.put(12, "1341wrû5");
    std::cout<<hm.get(12);
    system("pause");
    return 0;
}

