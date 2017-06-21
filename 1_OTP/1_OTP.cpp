#include "onetimepad.cpp"
#include "iostream"
#include <vector>
#include <fstream>
#include <locale>
#include <string>
#define string std::string
#define vector std::vector
#define endl std::endl
int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "RUSSIAN");
    cipher cipherclass;
    if (argc == 4)
    {
        string input = argv[1];
        string key = argv[2];
        string output = argv[3];
        if (input == output)
        {
            cout << "The same file is used as source and result. Continue anyway? (y/n)";
            char c;
            cin >> c;
            if (c == 'n')exit(2); else if (c != 'y')return 1;
        }
        if (!cipherclass.code(input, key, output))
        {
            cout << "Error while ciphering.";
            return 1;
        };
        cout << "Operation performed successfuly. Check file \"" << output << "\"";
    }
    else
        if (argc == 3) {
            string source = argv[1];
            string keypath = argv[2];
            if (source == keypath)
            {
                cout << "Are you sure you wish to replace source file? (y/n)";
                char c;
                cin >> c;
                if (c == 'n')exit(2); else if (c != 'y')return 1;
            }
            cout << '"' << argv[1] << "\" should be ciphered with \"" << argv[2] << '"' << endl;
            if (cipherclass.full(source,keypath))
            {
                cout << ("Key file \"" + keypath + "\" created");
            }
        }
        else
        {
            cout << "Wrong arguments. cipher <source file> <key file> <result file> for ciphering; cipher <source file> <key file> for key generation";  return 1;
        }
    return 0;
}