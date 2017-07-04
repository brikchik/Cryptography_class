#include <iostream>
#include <vector>
#include <string>
#include "util.cpp"
#include <locale>
#include <time.h>
#include <map>
#include <fstream>
#include <mutex>
#define cout std::cout
#define cin std::cin
#define thread std::thread
util utils;
struct KEY
{
    unsigned char key1[8];
    unsigned char key2[8];
    void init(unsigned long long x1, unsigned long long x2)
    {
        key1[0] = x1 & 0xFF00000000000000;
        key1[1] = x1 & 0x00FF000000000000;
        key1[2] = x1 & 0x0000FF0000000000;
        key1[3] = x1 & 0x000000FF00000000;
        key1[4] = x1 & 0x00000000FF000000;
        key1[5] = x1 & 0x0000000000FF0000;
        key1[6] = x1 & 0x000000000000FF00;
        key1[7] = x1 & 0x00000000000000FF;
        key2[0] = x2 & 0xFF00000000000000;
        key2[1] = x2 & 0x00FF000000000000;
        key2[2] = x2 & 0x0000FF0000000000;
        key2[3] = x2 & 0x000000FF00000000;
        key2[4] = x2 & 0x00000000FF000000;
        key2[5] = x2 & 0x0000000000FF0000;
        key2[6] = x2 & 0x000000000000FF00;
        key2[7] = x2 & 0x00000000000000FF;
    }
};
unsigned long long real_key1 = 0;
unsigned long long real_key2 = 0;
int a = 65536;//min 2^16
int b = 262144;//max 2^18
void doEncrypt()
{
    KEY deskey;
    srand(time(0));
    unsigned long long x1 = rand() % b + a;
    unsigned long long x2 = rand() % b + a;
    real_key1 = x1;
    real_key2 = x2;
    deskey.init(x1, x2);
    utils._des2(deskey.key1, deskey.key2);
}
std::vector<std::vector<byte>> vect1;
std::vector<std::vector<byte>> vect2;
void fillPart(int start, int finish, std::vector<std::vector<byte>> *vect1, bool encrypting, std::vector<byte> &left, std::vector<byte> &right)
{
    KEY _deskey;
    util _utils;
    for (int i = start; i < finish; i++) {
        _deskey.init(i, i);
        try
        {
            if (encrypting)_utils._des(left, _deskey.key1); else _utils._undes(right, _deskey.key2);
            vect1->push_back(_utils._result);
        }
        catch (...) { cout << "Something has gone wrong with DES!"; }
    }
};
std::vector<std::vector<byte>> vect_1;
std::vector<std::vector<byte>> vect_2;
std::vector<std::vector<byte>> vect_3;
std::vector<std::vector<byte>> vect_4;
std::vector<std::vector<byte>> Dvect_1;
std::vector<std::vector<byte>> Dvect_2;
std::vector<std::vector<byte>> Dvect_3;
std::vector<std::vector<byte>> Dvect_4;
void doBuild()
{
    std::ofstream fo1("log1.txt");
    std::ofstream fo2("log2.txt");
    KEY deskey;
    cout << "Started building tables\n";
    unsigned long clock_s = clock();
    for (int i = a; i < b; i++) {
        deskey.init(i, 0);
        try
        {
            utils._des(utils.left, deskey.key1);
            vect1.push_back(utils._result);
            fo1 << i << ':' << utils._result.data() << '\n';
        }
        catch (...) { cout << "Something has gone wrong"; }
    }
    cout << "Counting in 1 thread took: " << clock() - clock_s << '\n';
    clock_s = clock();
    thread b1(fillPart, a, a + (b - a) / 4.0, &vect_1, true, std::vector<byte>(utils.left), std::vector<byte>(utils.right));
    thread b2(fillPart, a + (b - a) / 4.0, a + (b - a) / 2.0, &vect_2, true, std::vector<byte>(utils.left), std::vector<byte>(utils.right));
    thread b3(fillPart, a + (b - a) / 2.0, a + (b - a) / 4.0 * 3, &vect_3, true, std::vector<byte>(utils.left), std::vector<byte>(utils.right));
    thread b4(fillPart, a + (b - a) / 4.0 * 3, b, &vect_4, true, std::vector<byte>(utils.left), std::vector<byte>(utils.right));
    b1.join();
    b2.join();
    b3.join();
    b4.join();
    cout << "Counting in 4 threads took: " << clock() - clock_s << '\n';
    cout << "Finished building E(k1,m) table\n";
    clock_s = clock();
    for (int j = a; j < b; j++)
    {
        deskey.init(0, j);
        try
        {
            utils._undes(utils.right, deskey.key2);
            vect2.push_back(utils._result);
            fo2 << j << ':' << utils._result.data() << '\n';
        }
        catch (...) {}
    }
    cout << "Counting in 1 thread took: " << clock() - clock_s << '\n';
    clock_s = clock();
    thread d1(fillPart, a, a + (b - a) / 4.0, &Dvect_1, true, std::vector<byte>(utils.left), std::vector<byte>(utils.right));
    thread d2(fillPart, a + (b - a) / 4.0, a + (b - a) / 2.0, &Dvect_2, true, std::vector<byte>(utils.left), std::vector<byte>(utils.right));
    thread d3(fillPart, a + (b - a) / 2.0, a + (b - a) / 4.0 * 3, &Dvect_3, true, std::vector<byte>(utils.left), std::vector<byte>(utils.right));
    thread d4(fillPart, a + (b - a) / 4.0 * 3, b, &Dvect_4, true, std::vector<byte>(utils.left), std::vector<byte>(utils.right));
    d1.join();
    d2.join();
    d3.join();
    d4.join();
    cout << "Counting in 4 threads took: " << clock() - clock_s << '\n';
    cout << "Finished building D(k2,c) table\n";
    fo1.close();
    fo2.close();
}
void doCompare()
{
    bool found = false;
    cout << "Merging vectors...\n";
    vect_1.insert(vect_1.end(), vect_2.begin(), vect_2.end());
    vect_1.insert(vect_1.end(), vect_3.begin(), vect_3.end());
    vect_1.insert(vect_1.end(), vect_4.begin(), vect_4.end());
    Dvect_1.insert(Dvect_1.end(), Dvect_2.begin(), Dvect_2.end());
    Dvect_1.insert(Dvect_1.end(), Dvect_3.begin(), Dvect_3.end());
    Dvect_1.insert(Dvect_1.end(), Dvect_4.begin(), Dvect_4.end());
    cout << vect1.size() << '_' << vect_1.size() << "<-E(k1,m) vector   " << vect2.size() << '_' << Dvect_1.size()<<"<-D(k2,c) vector" << " ... Success!\n";
    cout << "Looking for the correct key. This might take time...\n";
    for (int i = 0; i < b - a && !found; i++)
        for (int j = 0; j < b - a; j++)
            if (vect1.at(i) == vect2.at(j))
            {
                try {
                    cout << "K = " << i + a << "||" << j + a << " -> 112 bit key ready.";
                    KEY deskey;
                    deskey.init(i + a, j + a);
                    utils._undes(utils.right, deskey.key2);
                    std::vector<byte> temp = std::vector<byte>(utils._result);
                    utils._undes(temp, deskey.key1);
                    if (utils._result != utils.left)
                    {
                        cout << " False key. Looking for another one...\n";
                        continue;
                    }
                    cout << "Decode result: ";
                    cout << utils._result.data() << '\n';
                    found = true;
                    cout << "trying to encode again(check):\n";
                    cout << utils.left.data() << '\n';
                    utils._des(utils.left, deskey.key1);
                    cout << utils._result.data() << '\n';
                    temp = std::vector<byte>(utils._result);
                    utils._des(temp, deskey.key2);
                    cout << utils._result.data() << '\n';
                    break;
                }
                catch (...) {
                    cout << "Exception thrown (non-readable symbols?)\n";
                }
            }
}
int main()
{
    setlocale(LC_ALL, "RUSSIAN");
    doEncrypt();
    doBuild();
    doCompare();
    cout << "Real key: " << real_key1 << "||" << real_key2 << "\nKeys can be different but they'll work well if you encode messages without parity check option. The message was successfully decoded with them." << "\n_finished_";
    cin.get();
    return 0;
}