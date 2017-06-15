#include "rsa.cpp"
int main()
{
    setlocale(LC_ALL, "RUSSIAN");
    RSAclass rsa;
    std::cout << "Encrypting test.docx;\n";
    rsa.Encrypt("test.docx", "crypted.docx", "public.pem");
    std::cout << "Encrypted;\n";
    std::cout << "Decrypting crypted.docx;\n";
    rsa.Decrypt("crypted.docx", "Decrypted.docx", "private.pem");
    std::cout << "Decrypted;\n";
    system("cls");
    system("pause");
    return 0;
}