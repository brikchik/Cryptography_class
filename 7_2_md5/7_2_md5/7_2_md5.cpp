#include "../../Foper.cpp"
#include <openssl\md5.h>
#define blockLength 120
std::vector<unsigned char> Hash(std::vector<unsigned char> file) {
    std::vector<unsigned char> result;
    int size = file.size();
    int maxSize = size;
    while (maxSize % blockLength != 0)maxSize++;
    std::vector<unsigned char> block;
        MD5_CTX md5handler;
        unsigned char x[blockLength];
        unsigned char md5digest[MD5_DIGEST_LENGTH];
        for (int i = 0; i < maxSize / blockLength; i++)
        {
            for (int j = 0; j < blockLength; j++)
                if (j + blockLength*i < size)x[j] = file.at(j + blockLength * i); else x[j] = 0;
            MD5_Init(&md5handler);
            MD5_Update(&md5handler, x, blockLength);
            MD5_Final(md5digest, &md5handler);
            for (int i = 0; i < blockLength / 10; i++)result.push_back(md5digest[i]);//taking first bytes of md5 hash
        }
        return result;
}
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
    std::vector<unsigned char> hash = Hash(file.GetData());
    Foper outfile;
    outfile.GetData() = hash;
    outfile.write(output);
    std::cout << "demonstration finished\n";
    system("pause");
    return 0;
}