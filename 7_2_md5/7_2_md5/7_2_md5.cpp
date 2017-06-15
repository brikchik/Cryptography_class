#include "md5.h"
int main()
{
    Foper file;
    string input;
    std::cout << "MD5 hash function. Input source file name to count hash\n";
    std::cin >> input;
    if (!file.open(input))return 1;
    string output;
    std::cout << "Output file name: \n";
    std::cin >> output;
    md5class instance;
    std::vector<unsigned char> hash = instance.Hash(file.GetData());
    Foper outfile;
    outfile.GetData() = hash;
    outfile.write(output);
    std::cout << "demonstration finished\n";
    system("pause");
    return 0;
}