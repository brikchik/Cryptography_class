#include <vector>
#include <string>
#define string std::string
class rc4 {
private:
    int tableSize = 256;
    std::vector<unsigned char> key;
    std::vector<unsigned char> table;
    unsigned char a = 0;
    unsigned char b = 0;
    void swap(int i, int j)
    {
        int tempval = table.at(j);
        table.at(j) = table.at(i);
        table.at(i) = tempval;
    }
public:
    void ms()
    {
        for (int i = 0; i < tableSize; i++)
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
    }
    unsigned char get()
    {
        a = (a + 1) % tableSize;
        b = (b + table.at(b)) % tableSize;
        swap(a, b);
        unsigned char Ki = table.at((table.at(a) + table.at(b)) % tableSize);
        return Ki;
        system("pause");
    }
    void setKey(string &_key) { for (int i = 0; i < _key.size(); i++)key.push_back(_key[i]); };
};