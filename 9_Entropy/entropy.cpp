#include "EntropyCounter.h"
Foper file;
Foper outfile;
int main()
{
    EntropyCounter ec;
    std::cout << "plaintext entropy:\n";
    ec.count("../resources/file.txt",1);
    ec.count("../resources/file.txt",2);
    ec.count("../resources/file.txt",4);
    ec.count("../resources/file.txt",8);
    std::cout << "Encrypting file...\n";
    RSAclass rsac;
    rsac.Encrypt("file.txt", "Cfile.txt", "../resources/public.pem");
    std::cout<< "ciphertext (rsa, random public key):\n";
    ec.count("Cfile.txt", 1);
    ec.count("Cfile.txt", 2);
    ec.count("Cfile.txt", 4);
    ec.count("Cfile.txt", 8);
    std::cout << "hash file (md5):\n";
    md5class instance;
    if (!file.open((string)"file.txt"))return 1;
    outfile.GetData() = instance.Hash(file.GetData(), 32, 32);
    outfile.write((string)"Hfile.txt");
    ec.count("Hfile.txt", 1);
    ec.count("Hfile.txt", 2);
    ec.count("Hfile.txt", 4);
    ec.count("Hfile.txt", 8);
    std::cout << "hash file (RH hash function):\n";
    string input = "file.txt";
    if (!file.open(input))return 1;
    string output= "hafile.txt";
    rh rhclass;
    rhclass.setBlockLength(32);
    rhclass.setOutputSize(32);
    outfile.GetData() = rhclass.Hash(file.GetData());
    outfile.write(output);
    ec.count("hafile.txt", 1);
    ec.count("hafile.txt", 2);
    ec.count("hafile.txt", 4);
    ec.count("hafile.txt", 8);
    std::cout << "plaintext (2byte) with intersection:\n";
    ec.countIntersect("Hfile.txt", 2);
    outfile.write((string)"Zfile.txt");
    /*char* name = "Zfile.txt";
    HZIP zip=CreateZip((TCHAR*)name,NULL);
    ZipAdd(zip, (TCHAR*)"packedFile" , (TCHAR*)"file.txt");
    CloseZip(zip);
    Foper zipFile; zipFile.open((string)name);
    double difference = 1.0*zipFile.GetData().size()/file.GetData().size()*100;
    std::cout << "Archiving gives a file which is " << difference <<" from file.txt\n";*/
    system("pause");
    return 0;
}