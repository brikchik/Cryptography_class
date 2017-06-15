#include "imports.h"
int main()
{
    EntropyCounter ec;
    std::cout << "plaintext:\n";
    ec.count("file.txt",1);
    ec.count("file.txt",2);
    ec.count("file.txt",4);
    ec.count("file.txt",8);
    std::cout << "ciphertext (rsa, random public key):\n";
    RSAclass rsac;
    rsac.Encrypt("file.txt", "Cfile.txt", "public.pem");
    ec.count("Cfile.txt", 1);
    ec.count("Cfile.txt", 2);
    ec.count("Cfile.txt", 4);
    ec.count("Cfile.txt", 8);
    std::cout << "hash file (md5):\n";//hashed separately
    /*md5class md5;
    Foper file;
    if (!file.open((string)"file.txt"))return 1;
    std::vector<_byte> V=md5.Hash(file.GetData(), 32, 32);
    Foper outFile;
    outFile.GetData() = V;
    outFile.write((string)"Hfile.txt");*/
    ec.count("Hfile.txt", 1);
    ec.count("Hfile.txt", 2);
    ec.count("Hfile.txt", 4);
    ec.count("Hfile.txt", 8);
    system("pause");
    return 0;
}