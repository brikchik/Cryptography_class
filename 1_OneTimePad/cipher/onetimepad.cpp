#include "stdafx.h"
#include "iostream"
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
class Foper
{
private:
	vector<_byte> _data;
public:
	Foper()
	{
	}
	bool open(string &name)
	{
		ifstream _file(name, std::ios::in | std::ios::binary);
		if (!_file.is_open())
        {
            cout << "No such file: " << name<< endl;
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
				// write data to vector
			}
		}
		else {
			cout << "Empty file"; //if file is not empty
			_file.close();
		}
		return !_data.empty();
	}
	vector<_byte> &GetData() { return _data; }
	bool write(string &output)
	{
		std::ofstream outfile(output, std::fstream::out | std::fstream::trunc | std::fstream::binary);
		if (!outfile.is_open()) { cout << "Unable to open file for writing"; return false; };
		for (int i = 0; i < _data.size(); i++)
		{
			outfile.put((_byte)_data.at(i));
		}
		outfile.close();
		return true;
	}
};
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
			cout << errstr;
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
