#include "stdafx.h"
#include <locale>
#include <fstream>
#include <vector>
#include <random>
#include <iostream>
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
		mas.push_back(rand()%256);
	} //filling key vector
	return mas;
} //random key generation
int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "RUSSIAN");//for cyrillic symbols support
	if (!(argc == 3)) { cout << "Wrong arguments. keygen <source file> <key file>";  return 1; }
	string source = argv[1];
	string keypath = argv[2];
	if (source == keypath)
	{
		cout << "Are you sure you wish to replace source file? (y/n)";
		char c;
		cin >> c;
		if (c == 'n')exit(2); else if (c != 'y')return 1;
	}
	cout << '"' << argv[1]<< "\" should be ciphered with \"" << argv[2] << '"' << endl;
	if(write(keypath, full(source)))cout<<"Key file \""+keypath+"\" created"; //writing key
    return 0;
}