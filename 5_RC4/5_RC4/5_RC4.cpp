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
        std::cout << "\nВведите: 1-задание ключа вручную, 2-задание ключа из файла, 3-создание нового ключа и запись в файл key.txt, 4-шифрование файла, другое значение-выход\n";
        std::cin >> choice;
        if (choice == 1)
        {
            string key = "";
            std::cout << "Ключ: ";
            std::cin >> key;
            example.setKey(key);
        }else
        if (choice == 2)
        {
            string path = "";
            std::cout << "Путь к файлу ключа: ";
            std::cin >> path;
            example.setKeyFromFile(path);
        }else
        if (choice == 3)
        {
            int len = 0;
            std::cout << "Длина ключа: ";
            std::cin >> len;
            example.createKey(len);
        }else
        if (choice == 4)
        {
            string file = "";
            std::cout << "Имя исходного файла: ";
            std::cin >> file;
            string file2 = "";
            std::cout << "Имя выходного файла: ";
            std::cin >> file2;
            example.code(file, file2);
        }
        else
            running = false;
    }
    return 0;
}