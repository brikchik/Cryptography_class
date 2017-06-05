#include <locale>
#include "rc4.cpp"
int main()
{
    setlocale(LC_ALL, "RUSSIAN");
    system("cls");
    rc4 example;
    bool running = true;
    while (running)
    {
        int choice = 0;
        std::cout << "\n�������: 1-������� ����� �������, 2-������� ����� �� �����, 3-�������� ������ ����� � ������ � ���� key.txt, 4-���������� �����, ������ ��������-�����\n";
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
        }
        else
            running = false;
    }
    return 0;
}