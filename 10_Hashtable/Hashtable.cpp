#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include "../shared/rh.h"
#include "../shared/md5.h"
#include <vector>
#include <locale>
#include <fstream>
#pragma comment(linker, "/STACK:65536")
//http://www.algolist.net/Data_structures/Hash_table/Chaining
//http://www.firststeps.ru/mfc/winapi/r.php?158
class filePar
{
public:
    string name="";
    std::vector<string> attrib;
    filePar() {}
    filePar(string a, std::vector<string> attributes) 
    {
        name = a;
        attrib = attributes;
    }
};
class LinkedHashEntry {
private:
    string key;
    filePar value=filePar();
    LinkedHashEntry *next=NULL;
public:
    LinkedHashEntry(string key, filePar value) {
        this->key = key;
        this->value = value;
        this->next = NULL;
    }
    string &getKey() {
        return key;
    }
    filePar &getValue() {
        return value;
    }
    void setValue(filePar value) {
        this->value = value;
    }
    LinkedHashEntry *getNext() {
        return next;
    }
    void setNext(LinkedHashEntry *next) {
        this->next = next;
    }
};
const int TABLE_SIZE = 256;
int maxAlpha = 3;
class HashMap {
private:
    md5class hash1;
    rh hash0;
    int hashType = 1;//md5
    int Hash(string key) { 
        int result = 0;
        std::vector<unsigned char> temp;
        std::vector<unsigned char> temp2;
        for (int i = 0; i < key.length(); i++)temp.push_back(key[i]);
        if (hashType == 1)temp2 = hash1.Hash(temp, temp.size(), 2);
        else
            temp2 = hash0.Hash(temp, temp.size(), 2);
        result += temp2.at(0)<<8;
        result += temp2.at(1);
        return result%TABLE_SIZE;
    }
    LinkedHashEntry **table;
    int alpha = 0;
public:
    HashMap() {
        table = new LinkedHashEntry*[TABLE_SIZE];
        for (int i = 0; i < TABLE_SIZE; i++)
            table[i] = NULL;
    }
    filePar get(string key) {
        int hash = Hash(key);
        if (table[hash] == NULL)
            return filePar();
        else {
            LinkedHashEntry *entry = table[hash];
            while (entry != NULL && entry->getKey() != key)
                entry = entry->getNext();
            if (entry == NULL)
                return filePar();
            else
                return entry->getValue();
        }
    }
    int getAlpha() { return alpha; }
    void rebuild(int hash)
    {
        std::cout << "rebuilding";
        hashType = hash;
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            LinkedHashEntry* newTable[TABLE_SIZE];
            string tempname = table[i]->getKey();
            //newTable[Hash(tempname)]=table[i];
            
        }
    }
    void put(string key, filePar value) {
        int hash = Hash(key);
        int number = 0;
        if (table[hash] == NULL)
        {
            number++;
            table[hash] = new LinkedHashEntry(key, value);
        }
        else {
            LinkedHashEntry *entry = table[hash];
            while (entry->getNext() != NULL)
            {
                entry = entry->getNext();
                number++;
            }
            if (entry->getKey() == key)
                entry->setValue(value);
            else
            {
                entry->setNext(new LinkedHashEntry(key, value));
                number++;
            }
        }
        if (number > alpha)alpha++;
        if (alpha > maxAlpha)rebuild(0);
    }
    void remove(string key) {
        int hash = Hash(key);
        if (table[hash] != NULL) {
            LinkedHashEntry *prevEntry = NULL;
            LinkedHashEntry *entry = table[hash];
            while (entry->getNext() != NULL && entry->getKey() != key) {
                prevEntry = entry;
                entry = entry->getNext();
            }
            if (entry->getKey() == key) {
                if (prevEntry == NULL) {
                    LinkedHashEntry *nextEntry = entry->getNext();
                    delete entry;
                    table[hash] = nextEntry;
                }
                else {
                    LinkedHashEntry *next = entry->getNext();
                    delete entry;
                    prevEntry->setNext(next);
                }
            }
        }
    }
    void printMap(string path="") 
    {
        bool toFile = false;
        std::ofstream outfile;
        if (path != "")
        {
            outfile.open(path.c_str());
            toFile = true;
        }
        for (int i = 0; i < TABLE_SIZE; i++)
        {
            if (table[i] != NULL)
            {
                LinkedHashEntry *entry = table[i];
                if(toFile)outfile << i << ": "; else std::cout << i << ": ";
                while (entry != NULL)
                {
                    if (toFile)outfile << entry->getValue().name << ' '; else std::cout << entry->getValue().name << ' ';
                    entry = entry->getNext();
                }
                if (toFile)outfile << '\n'; else std::cout << '\n';
                delete entry;
            }
        }
        if (toFile)outfile.close();
    }
    ~HashMap() {
        for (int i = 0; i < TABLE_SIZE; i++)
            if (table[i] != NULL) {
                LinkedHashEntry *prevEntry = NULL;
                LinkedHashEntry *entry = table[i];
                while (entry != NULL) {
                    prevEntry = entry;
                    entry = entry->getNext();
                    delete prevEntry;
                }
            }
        delete[] table;
    }
};

HashMap hm;
void addFolder(string &path) 
{
    WIN32_FIND_DATA FindFileData;
    HANDLE hf;
    hf = FindFirstFile(path.c_str(), &FindFileData);
    while (FindNextFile(hf, &FindFileData))
    {
        string fpath = FindFileData.cFileName;
        hm.put(path + '/' + fpath , filePar(path+'/'+fpath, std::vector<string>(FindFileData.dwFileAttributes)));
        if ((FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) && fpath != "." && fpath != "..")
        {
            path.resize(path.size() - 1);
            addFolder(path + '/' + fpath + "/*");
        }
        std::cout << hm.getAlpha();
    }
};
int main()
{
    setlocale(LC_ALL, "RUSSIAN");
    std::cin.clear();
    string path = "D://hack/*";
    addFolder(path);
    try { hm.printMap("res.txt"); }
    catch (...) { std::cout << "error at printMap()"; };
    system("pause");
    return 0;
}