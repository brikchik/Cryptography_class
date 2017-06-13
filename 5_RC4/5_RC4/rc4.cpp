#include "imports.h"
class rc4 {
private:
    const int blockLength = 8; //1byte block
    int tableSize = 0;
    vector<_byte> key;
    vector<_byte> table;
    _byte a = 0;
    _byte b = 0;
    void swap(int i, int j)
    {
        int tempval = table.at(j);
        table.at(j) = table.at(i);
        table.at(i) = tempval;
    }
    void ms()
    {
        for (int i = 0; i < pow(2, blockLength); i++)
        {
            table.push_back(i);
        }
        int j = 0;
        int tempval = 0;
        for (int i = 0; i < tableSize; i++)
        {
            j = (j + table.at(i) + key.at(i%key.size())) % 256;
            swap(i, j);
        }
        for (int i = 0; i < tableSize; i++)std::cout << (int)table.at(i) << ' ';
        system("pause");
    }
    _byte get()
    {
        a = (a + 1) % tableSize;
        b = (b + table.at(b)) % tableSize;
        swap(a, b);
        _byte Ki = table.at((table.at(a) + table.at(b)) % tableSize);
        return Ki;
    }
public:
    rc4() {
        tableSize = pow(2, blockLength);
    };
    void setKey(string &_key) { for (int i = 0; i < _key.size(); i++)key.push_back(_key[i]); };
    void setKeyFromFile(string &path)
    {
        Foper keyfile;
        if (!keyfile.open(path))
        {
            std::cout << "Key file not found";
            return;
        }
        key = keyfile.GetData();
    }
    void createKey(int length) //file is required as copying from console is inconvenient
    {
        key.clear();
        Foper keyfile;
        for (int i = 0; i < length; i++)
        {
            _byte b = rand() % 256;
            key.push_back(b);
            std::cout << b;
        }
        keyfile.GetData() = key;
        keyfile.write((string)"key.txt");
    }
    void code(string inputpath, string outputpath)
    {
        Foper input;
        Foper output;
        if (!input.open(inputpath))
        {
            std::cout << "Key file not found";
            return;
        }
        a = 0;
        b = 0;
        table.clear();

        ms();
        for (int i = 0; i < input.GetData().size(); i++)
            output.GetData().push_back(input.GetData().at(i) ^ get());
        output.write(outputpath);
    }
};