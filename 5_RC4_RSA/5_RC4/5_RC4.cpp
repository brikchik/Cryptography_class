#include <locale>
#include "rc4.cpp"
#include "rsa.cpp"
int main()
{
    setlocale(LC_ALL, "RUSSIAN");
    system("cls");
    rc4 example;
    rsa rsaclass;
    bool running = true;
    while (running)
    {
        int choice = 0;
        std::cout << "\nRC4. �������: 1-������� ����� �������, 2-������� ����� �� �����, 3-�������� ������ ����� � ������ � ���� key.txt, 4-���������� �����, ������ ��������-�����\n"<<"RSA. ������� 5\n";
        std::cin >> choice;
        if (choice == 1)
        {
            string key = "";
            std::cout << "����: ";
            std::cin >> key;
            example.setKey(key);
        }else
        if (choice == 2)
        {
            string path = "";
            std::cout << "���� � ����� �����: ";
            std::cin >> path;
            example.setKeyFromFile(path);
        }else
        if (choice == 3)
        {
            int len = 0;
            std::cout << "����� �����: ";
            std::cin >> len;
            example.createKey(len);
        }else
        if (choice == 4)
        {
            string file = "";
            std::cout << "��� ��������� �����: ";
            std::cin >> file;
            string file2 = "";
            std::cout << "��� ��������� �����: ";
            std::cin >> file2;
            example.code(file, file2);
        }else
            if (choice == 5)
            {
                char plainText[4096] = "My ciphered text - ������";
                unsigned char  encrypted[4096] = {};
                unsigned char decrypted[4096] = {};
                std::cout << "Source: " << plainText << std::endl;
                int encrypted_length = rsaclass.public_encrypt((_byte*)plainText, strlen(plainText), (_byte*)"public.pem",true, encrypted);
                std::cout << "encrypted: "<<encrypted<<std::endl;
                int decrypted_length = rsaclass.private_decrypt(encrypted, encrypted_length, (_byte*)"private.pem",true, decrypted);
                std::cout << "decrypted: " << decrypted << std::endl;
                encrypted_length = rsaclass.private_encrypt((_byte*)plainText, strlen((char*)plainText), (_byte*)"private.pem",true, encrypted);
                std::cout << "encrypted: " << encrypted << std::endl;
                decrypted_length = rsaclass.public_decrypt(encrypted, encrypted_length, (_byte*)"public.pem",true, decrypted);
                std::cout << "decrypted: " << decrypted << std::endl;
            }
        else
            running = false;
    }
    return 0;
}