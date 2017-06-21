#include "imports.h"
#define cout std::cout
int main()
{
    setlocale(LC_ALL, "RUSSIAN");
    RSAclass rsa;
    cout << "1-example(resources/test.docx encoded and decoded with resources/keys), 2-encode with public.pem, 3-decode with private.pem";
    int choice=0;
    std::cin >> choice;
    string sourcefile="";
    string destfile="";
    if (choice == 1)
    {
        cout << "Encrypting test.docx;\n";
        rsa.Encrypt("../resources/test.docx", "crypted.docx", "../resources/public.pem");
        cout << "Encrypted;\n";
        cout << "Decrypting crypted.docx;\n";
        rsa.Decrypt("crypted.docx", "Decrypted.docx", "../resources/private.pem");
        cout << "Decrypted;\n";
    }else
        if (choice == 2)
        {
            cout << "Input file: ";
            std::cin >> sourcefile;
            cout << "Output file: ";
            std::cin >> destfile;
            rsa.Encrypt(sourcefile, destfile, "public.pem");
        }else
            if (choice == 3)
            {
                cout << "Input file: ";
                std::cin >> sourcefile;
                cout << "Output file: ";
                std::cin >> destfile;
                rsa.Decrypt(sourcefile, destfile, "private.pem");
            }
    system("cls");
    system("pause");
    return 0;
}