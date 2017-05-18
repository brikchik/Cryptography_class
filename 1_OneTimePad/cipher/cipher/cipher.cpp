#define _byte unsigned char
#include "stdafx.h"
#include "iostream"
#include <vector>
#include <fstream>
#include <locale>
using namespace std;

bool write(string &output, vector<unsigned char> mas)
{
	ofstream outfile(output, fstream::out | fstream::trunc | fstream::binary); //creating file
	if (!outfile.is_open())return false;
	for (int i = 0; i < mas.size(); i++)
	{
		outfile.put((unsigned char)mas.at(i));
		//adding value to vector
	}
	outfile.close();
	return true;
} // writing random to file
vector<unsigned char> full(string &source)
{
	ifstream _file(source, ios::in | ios::binary);
	if (!_file.is_open()) { cout << "Wrong input file"; exit(1); }
	_file.seekg(0, ios::end);
	int size = _file.tellg();
	_file.close();
	//getting file size
	vector<unsigned char> mas; //key vector
	for (int i = 0; i < size; i++)
	{
		mas.push_back(rand() % 256);
	} //filling key vector
	return mas;
} //random key generation
// Functions for key generation

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
		if (!code(input, key, output)) { cout << "Error while ciphering."; return 1; };
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
			if (write(keypath, full(source)))cout << "Key file \"" + keypath + "\" created"; //writing key
		}
	else 
	{
	cout << "Wrong arguments. cipher <source file> <key file> <result file> for ciphering; cipher <source file> <key file> for key generation";  return 1;
	}
	return 0;
}