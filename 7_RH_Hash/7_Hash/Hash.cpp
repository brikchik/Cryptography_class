#include "rh.h"
int main()
{
    Foper file;
    string input;
    std::cout << "RHash function. Input source file name to count hash\n";
    //std::cin >> input;
    input = "file.txt";
    if (!file.open(input))return 1;
    string output;
    std::cout << "Output file name: \n";
    //std::cin >> output;
    setBlockLength(32);
    setOutputSize(32);
    output = "hafile.txt";
    std::vector<unsigned char> hash = Hash(file.GetData());
    Foper outfile;
    outfile.GetData() = hash;
    outfile.write(output);
    std::cout << "demonstration finished\n";
    system("pause");
    return 0;
}