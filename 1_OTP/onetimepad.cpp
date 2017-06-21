#pragma once
#include "../SHARED/Foper.cpp"
#include <Windows.h>
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
    bool full(string &source, string &output)
    {
        Foper _file;
        if (!_file.open(source))
        {
            cout << "Wrong input file";
            exit(1);
        }
        Foper _outfile;
        for (int i = 0; i < _file.GetData().size(); i++)
        {
            _outfile.GetData().push_back(rand() % 256);
        }
        _outfile.write(output);
        return true;
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
