#include "iostream"
#include "../../Foper.cpp"
#include <vector>
#include <fstream>
#include <locale>
#include <string>
#define _byte unsigned char
#define string std::string
#define vector std::vector
#define ifstream std::ifstream
#define outparams std::fstream::out | std::fstream::trunc | std::fstream::binary
#define cout std::cout
#define endl std::endl
#define cin std::cin
class cipher {
public:
    cipher()
    {
    }
    bool write(string &output, vector<_byte> mas)
    {
        std::ofstream outfile(output, outparams);
        if (!outfile.is_open())return false;
        for (int i = 0; i < mas.size(); i++)
        {
            outfile.put((_byte)mas.at(i));
        }
        outfile.close();
        return true;
    }
    vector<_byte> full(string &source)
    {
        ifstream _file(source, std::ios::in | std::ios::binary);
        if (!_file.is_open())
        {
            cout << "Wrong input file";
            exit(1);
        }
        _file.seekg(0, std::ios::end);
        int size = _file.tellg();
        _file.close();
        vector<_byte> mas;
        for (int i = 0; i < size; i++)
        {
            mas.push_back(rand() % 256);
        }
        return mas;
    }
    bool code(string &inpath, string &keypath, string &outpath)
    {
        if (!_input.open(inpath))return false;
        if (!_key.open(keypath))return false;
        if (!(_key.GetData().size() == _input.GetData().size()))
        {
            cout << "Different key-plaintext length";
            char* errstr = "";
            cin >> errstr;
            return false;
        }
        for (int i = 0; i < _input.GetData().size(); i++)
        {
            _output.GetData().push_back(_input.GetData().at(i) ^ _key.GetData().at(i));
        }
        if (!_output.write(outpath))return false;
        return true;
    }
private:
    Foper _input;
    Foper _key;
    Foper _output;
};
