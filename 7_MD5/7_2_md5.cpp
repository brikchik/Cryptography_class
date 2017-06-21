#include "../SHARED/md5.h"
int main()
{
    Foper file;
    string input;
    std::cout << "MD5 hash function. Input source file name to count its hash\n";
    std::cin >> input;
    if (!file.open(input))return 1;
    string output;
    std::cout << "Output file name: \n";
    std::cin >> output;
    md5class instance;
    std::vector<unsigned char> hash = instance.Hash(file.GetData(),file.GetData().size(),64);
    Foper outfile;
    outfile.GetData() = hash;
    outfile.write(output);
    std::cout << "demonstration finished\n";
    system("pause");
    return 0;
}