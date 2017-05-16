#define _byte unsigned char
#include "stdafx.h"
#include "iostream"
#include <vector>
#include <fstream>
#include <locale>
using namespace std;

class Foper // class for file works (read,write)
{
private:
	std::vector<_byte> _data;
public:
	Foper()
	{
	}
	bool open(std::string &name)	//read from file function
	{
		ifstream _file(name,ios::in|ios::binary);
		if (!_file.is_open()) { cout << "No such file: " << name<< endl; return false; };
		_file.seekg(0, ios::end);
		int size = _file.tellg();
		_file.seekg(0, ios::beg); 
		//finding file size
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
	std::vector<_byte> &GetData() { return _data; }
	bool write(string &output)	//writing data to file
	{
		ofstream outfile(output, fstream::out | fstream::trunc | fstream::binary);
		if (!outfile.is_open()) { cout << "Unable to open file for writing"; return false; };
		for (int i = 0; i < _data.size(); i++)
		{
			outfile.put((_byte)_data.at(i));
			//adding value to file
		}
		outfile.close();
		return true;
	}
};

bool code(string &inpath, string &keypath, string &outpath) 
{
	Foper input;
	Foper key;
	Foper output;
	if (!input.open(inpath))return false;
	if (!key.open(keypath))return false;
	if (key.GetData().size() != input.GetData().size()) { cout << "Different key-plaintext length"; char* errstr = ""; cin >> errstr; cout << errstr; return false; }
	//checking key and plaintext size
	for (int i = 0; i < input.GetData().size(); i++)
	{
		output.GetData().push_back(input.GetData().at(i) ^ key.GetData().at(i));
		//adding coded byte to vector
	} //ciphering
	if (!output.write(outpath))return false;
	//writing result to file
	return true;
} //ciphering function

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "RUSSIAN");
	if (!(argc == 4)) { cout << "Wrong arguments. cipher <source file> <key file> <result file>";  return 1; }
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
	if (!code(input, key, output)) { cout << "Error while ciphering."; return 1; };
	cout << "Operation performed successfuly. Check file \"" << output << "\"";
	return 0;
}