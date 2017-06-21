#include "aes.cpp"
int main()
{
    aes_class aes;
    system("cls");
    std::cout << "Input command (gen_key/set_key/enc/dec): " << std::endl;
    string mode = "";
    std::cin >> mode;
    std::cout << "Mode: " << mode << std::endl;
    if (mode == "set_key")
    {
        char key_arr[32];
        std::cin >> key_arr;
        if (!aes.set_key((unsigned char*)key_arr))return -2;
        std::cout << "Key set.\nInput mode: ";
        std::cin >> mode;
        std::cout << "Mode: " << mode << std::endl;
    }
    else
        if (mode == "gen_key" || mode == "create_key")
        {
            std::cout << "Input key length: ";
            int length;
            std::cin >> length;
            aes.gen_key(length);
            if (mode == "create_key")
            {
                Foper keyfile;
                keyfile.GetData() = aes.keystore;
                keyfile.write(string("key.txt"));
            }
            std::cout << "Key set.\nInput mode: ";
            std::cin >> mode;
            std::cout << "Mode: " << mode << std::endl;
        }
    string source = "";
    std::cout << "Source: ";
    std::cin >> source;
    std::cout << std::endl;
    string dest = "";
    std::cout << "Destination: ";
    std::cin >> dest;
    std::cout << std::endl;
    if (mode == "enc") {
        aes.encrypt(source, dest);
    }
    if (mode == "dec") {
        aes.decrypt(source, dest);
    }
    system("pause");
    return 0;
}