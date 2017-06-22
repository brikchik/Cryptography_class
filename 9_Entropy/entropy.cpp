#include "EntropyCounter.h"
#include "../THIRD-PARTY/src/zip.h"
#include "../THIRD-PARTY/src/zip.cpp"
Foper file;
double Zip(TCHAR *archiveName, TCHAR *sourceFile)
{
    file.open(string(sourceFile));
    HZIP hz = CreateZip(archiveName, 0);
    ZipAdd(hz, "test", sourceFile);
    CloseZip(hz);
    Foper zippedfile;
    zippedfile.open(string(archiveName));
    double ratio = (double)file.GetData().size() / (double)zippedfile.GetData().size();
    return ratio;
}
int main()
{
    EntropyCounter ec;
    bool running = true;
    while (running) {
        char* filepath="";
        string name;
        int choice = -1;
        std::cout << "1-print all available entropy tests with source file, 2-get manual entropy value, any key-quit: ";
        std::cin >> choice;
        if (!(choice == 1) && !(choice == 2))running = false;
        else
        {
            std::cout << "source file name: ";
            std::cin >> name;
            filepath = (char*)name.c_str();
        }
        if (choice == 1)
        {
            std::cout << "plaintext entropy:\n";
            ec.count(filepath, 1);
            ec.count(filepath, 2);
            ec.count(filepath, 4);
            ec.count(filepath, 8);
            std::cout << "plaintext (2byte) with intersection:\n";
            ec.countIntersect(filepath);
            std::cout << "Encrypting file...\n";
            RSAclass rsac;
            rsac.Encrypt(filepath, "Cfile.txt", "public.pem");
            std::cout << "ciphertext (rsa, random public key):\n";
            ec.count("Cfile.txt", 1);
            ec.count("Cfile.txt", 2);
            ec.count("Cfile.txt", 4);
            ec.count("Cfile.txt", 8);
            std::cout << "ciphertext (rsa, random public key) (2byte) with intersection:\n";
            ec.countIntersect("Cfile.txt");
            std::cout << "hash file (md5):\n";
            md5class instance;
            if (!file.open((string)filepath))return 1;
            Foper outfile;
            outfile.GetData() = instance.Hash(file.GetData(), 32, 32);
            outfile.write((string)"MD5file.txt");
            ec.count("MD5file.txt", 1);
            ec.count("MD5file.txt", 2);
            ec.count("MD5file.txt", 4);
            ec.count("MD5file.txt", 8);
            std::cout << "hash file (md5) (2byte) with intersection:\n";
            ec.countIntersect("MD5file.txt");
            std::cout << "hash file (RH hash function):\n";
            if (!file.open(string(filepath)))return 1;
            rh rhclass;
            rhclass.setBlockLength(32);
            rhclass.setOutputSize(32);
            outfile.GetData() = rhclass.Hash(file.GetData());
            string output = "hafile.txt";
            outfile.write(output);
            ec.count("hafile.txt", 1);
            ec.count("hafile.txt", 2);
            ec.count("hafile.txt", 4);
            ec.count("hafile.txt", 8);
            std::cout << "hash file (RHash) (2byte) with intersection:\n";
            ec.countIntersect("hafile.txt");
            std::cout << "plaintext zip ratio: " << Zip("plain.zip", filepath) << '\n';
            std::cout << "rsa_text zip ratio: " << Zip("rsa.zip", "Cfile.txt") << '\n';
            std::cout << "md5_text zip ratio: " << Zip("md5.zip", "MD5file.txt") << '\n';
            std::cout << "rhash_text zip ratio: " << Zip("rhash.zip", "hafile.txt") << '\n';
            std::cout << std::endl;
            system("pause");
            system("cls");
        }
        else
            if (choice == 2)
            {
                int choice2 = 0;
                std::cout << "Count entropy: 1-plaintext, 2-rsa crypted file, 3-md5 hashed file, 4-RHash hashed file, 5-ZIP archived file\n";
                std::cin >> choice2;
                if (choice2 == 5)std::cout << "zip ratio(source/packed): " << Zip("zip.zip", filepath) << '\n';
                else
                {
                    int num = -1;
                    std::cout << "1,2,3,4 -> number of bytes per block, 0 -> 2byte with intersection: ";
                    std::cin >> num;
                    if (choice2 == 1)
                    {
                        if (num == 0)
                        {
                            std::cout << "plaintext entropy: ";
                            ec.countIntersect(filepath);
                        }
                        else {
                            std::cout << "plaintext entropy: ";
                            ec.count(filepath, num);
                        }
                    }
                    if (choice2 == 2)
                    {
                        RSAclass rsac;
                        rsac.Encrypt(filepath, "Cfile.txt", "public.pem");
                        std::cout << "ciphertext (rsa, public.pem key from file):\n";
                        filepath = "Cfile.txt";
                        if (num == 0)
                        {
                            std::cout << "rsa entropy: ";
                            ec.countIntersect(filepath);
                        }
                        else {
                            std::cout << "rsa_text entropy: ";
                            ec.count(filepath, num);
                        }
                    }
                    if (choice2 == 3)
                    {
                        md5class instance;
                        if (!file.open((string)filepath))return 1;
                        Foper outfile;
                        outfile.GetData() = instance.Hash(file.GetData(), 32, 32);
                        outfile.write((string)"MD5file.txt");
                        filepath = "MD5file.txt";
                        if (num == 0)
                        {
                            std::cout << "md5 entropy: ";
                            ec.countIntersect(filepath);
                        }
                        else {
                            std::cout << "md5 hash entropy: ";
                            ec.count(filepath, num);
                        }
                    }
                    if (choice2 == 4)
                    {
                        rh rhclass;
                        Foper outfile;
                        rhclass.setBlockLength(32);
                        rhclass.setOutputSize(32);
                        outfile.GetData() = rhclass.Hash(file.GetData());
                        filepath = "hafile.txt";
                        outfile.write(string(filepath));
                        if (num == 0)
                        {
                            std::cout << "RHash entropy: ";
                            ec.countIntersect(filepath);
                        }
                        else
                        {
                            std::cout << "RHash hash entropy: ";
                            ec.count(filepath, num);
                        }
                    }
                }
                std::cout << std::endl;
                system("pause");
                system("cls");
            };
    };
    return 0;
}