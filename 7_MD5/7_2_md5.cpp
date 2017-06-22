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
    Foper outfile;
    for (int i = 0; i<instance.Hash(file.GetData(),120,12).size(); i++)
        outfile.GetData().push_back(instance.Hash(file.GetData(),120,12).at(i));
    outfile.write(output);
    std::cout << "demonstration finished\n";
    system("pause");
    return 0;
}