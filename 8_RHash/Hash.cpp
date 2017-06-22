#include "../shared/rh.h"
int main()
{
    Foper file;
    string input = "file.txt";
    std::cout << "RHash function. Input source file name to count hash\n";
    std::cin >> input;
    if (!file.open(input))return 1;
    string output = "hafile.txt";
    std::cout << "Output file name: \n";
    std::cin >> output;
    rh rhclass;
    Foper outfile;
    for(int i=0;i<rhclass.Hash(file.GetData()).size();i++)
        outfile.GetData().push_back(rhclass.Hash(file.GetData()).at(i));
    outfile.write(output);
    std::cout << "demonstration finished\n";
    system("pause");
    return 0;
}