#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#define _byte unsigned char
#define string std::string
#define ifstream std::ifstream
class Foper
{
private:
    std::vector<unsigned char> _data;
public:
    bool open(string &name)
    {
        ifstream _file(name, std::ios::in | std::ios::binary);
        if (!_file.is_open())
        {
            std::cout << "No such file: " << name << std::endl;
            return false;
        }

        _file.seekg(0, std::ios::end);
        int size = _file.tellg();
        _file.seekg(0, std::ios::beg);

        if (size != 0) {
            _byte byte;
            for (int i = 0; i < size; i++)
            {
                byte = (_byte)(_file.get());
                _data.push_back(byte);
            }
        }
        else {
            std::cout << "Empty file";
            _file.close();
        }
        return !_data.empty();
    }
    std::vector<unsigned char> &GetData() { return _data; }
    bool write(string &output)
    {
        std::ofstream outfile(output, std::fstream::out | std::fstream::trunc | std::fstream::binary);
        if (!outfile.is_open()) { std::cout << "Unable to open file for writing"; return false; };
        for (int i = 0; i < _data.size(); i++)
        {
            outfile.put((_byte)_data.at(i));
        }
        outfile.close();
        return true;
    }
};