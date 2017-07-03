#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include "../shared/rh.h"
#include "../shared/md5.h"
#include <vector>
#include <locale>
#include <fstream>
#include <exception>
#pragma comment(linker, "/STACK:65536")
int TABLE_SIZE = 2048; //11bit hash
//http://www.algolist.net/Data_structures/Hash_table/Chaining
//http://www.firststeps.ru/mfc/winapi/r.php?158
class filePar
{
public:
    string name = "";
    std::vector<string> attrib;
    filePar() {}
    filePar(string a, std::vector<string> attributes)
    {
        name = a;
        attrib = attributes;
    }
    ~filePar()
    {
        attrib.clear();
    }
};
class LinkedHashEntry {
private:
    string key;
    filePar value = filePar();
    LinkedHashEntry *next = NULL;
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
class HashMap {
private:
    double MAX_ALPHA = 25;
    md5class hash1;
    rh hash0;
    int Hash(string key) {
        int result = 0;
        std::vector<unsigned char> temp;
        std::vector<unsigned char> temp2;
        for (int i = 0; i < key.length(); i++)temp.push_back(key[i]);
        if (hashType == 1)temp2 = hash1.Hash(temp, temp.size(), 2);
        else
        {
            temp2 = hash0.Hash(temp, temp.size(), 4);
        }
        result += temp2.at(0) << 8;
        result += temp2.at(1);
        return result%TABLE_SIZE;
    }
    LinkedHashEntry **table;
public:
    bool hashType = false;
    int alphaInRow = 0;
    int hashCount = 0;
    int count = 0;
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
    double getMAX_ALPHA() { return MAX_ALPHA; }
    void setMAX_ALPHA(double d) { MAX_ALPHA = d; }
    int getAlpha() { return alphaInRow; }
    int CountHashes() { return hashCount; }
    int Count() { return count; }
    void put(string key, filePar value) {
        int hash = Hash(key);
        int number = 0;
        if (table[hash] == NULL)
        {
            number++;
            table[hash] = new LinkedHashEntry(key, value);
            hashCount++;
            count++;
        }
        else {
            LinkedHashEntry *entry = table[hash];
            while (entry->getNext() != NULL)
            {
                entry = entry->getNext();
            }
            if (entry->getKey() == key)
                entry->setValue(value);
            else
            {
                entry->setNext(new LinkedHashEntry(key, value));
                number++;
                count++;
            }
        }
        if (number > alphaInRow)alphaInRow++;
    }
    void remove(string &key) {
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
    void printMap(string path = "")
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
                if (toFile)outfile << i << ": "; else std::cout << i << ": ";
                while (entry != NULL)
                {
                    if (toFile)outfile << entry->getValue().name << "; "; else std::cout << entry->getValue().name << ' ';
                    entry = entry->getNext();
                }
                if (toFile)outfile << '\n'; else std::cout << '\n';
                delete entry;
            }
        }
        if (toFile)outfile.close();
    }
    double avg()
    {
        double alpha;
        alpha = 1.0*Count() / CountHashes();
        return alpha;
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
HashMap* hm;
void addFolder(string &path)
{
    WIN32_FIND_DATA FindFileData;
    HANDLE hf;
    hf = FindFirstFileA((path + "/*").c_str(), &FindFileData);
    while (FindNextFile(hf, &FindFileData))
    {
        string fpath = FindFileData.cFileName;
        std::vector<string> attributes;
        if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)attributes.push_back("Directory"); else attributes.push_back("File");
        if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)attributes.push_back("Hidden"); else attributes.push_back("Open");
        if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)attributes.push_back("Readonly"); else attributes.push_back("ReadWrite");
        if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)attributes.push_back("SystemFile"); else attributes.push_back("UserFile");
        if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_TEMPORARY)attributes.push_back("Temporary"); else attributes.push_back("Constant");
        if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_ENCRYPTED)attributes.push_back("Encrypted"); else attributes.push_back("Plain");
        if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_COMPRESSED)attributes.push_back("Compressed"); else attributes.push_back("Not compressed");
        if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_OFFLINE)attributes.push_back("Offline access"); else attributes.push_back("Network access");
        SYSTEMTIME stUTC;
        FileTimeToSystemTime(&FindFileData.ftCreationTime, &stUTC);
        attributes.push_back("Created: " + stUTC.wSecond + ':' + stUTC.wMinute + ':' + stUTC.wHour + '_' + stUTC.wDay + '.' + stUTC.wMonth + '.' + stUTC.wYear);
        FileTimeToSystemTime(&FindFileData.ftLastAccessTime, &stUTC);
        attributes.push_back("Last accessed: " + stUTC.wSecond + ':' + stUTC.wMinute + ':' + stUTC.wHour + '_' + stUTC.wDay + '.' + stUTC.wMonth + '.' + stUTC.wYear);
        if (!FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            unsigned long fsize = (FindFileData.nFileSizeHigh * (MAXDWORD + 1)) + FindFileData.nFileSizeLow;
            attributes.push_back("Size: " + fsize);
        }
        hm->put(path + ('/' + fpath), filePar(path + ('/' + fpath), attributes));
        if ((FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) && fpath != "." && fpath != "..")
        {
            addFolder(path + ('/' + fpath));
        }
    }
};
void rebuild(bool hash, string &path)
{
    std::cout << "rebuilding_" << (hash) ? "in_md5" : 0 << '\n';
    hm->alphaInRow = 0;
    hm->hashCount = 0;
    hm->hashType = hash;
    try { addFolder(path); }
    catch (...) { std::cout << "Something went wrong. Try another hash function\n"; return; }
    std::cout << "After rebuild: " << hm->avg() << "\n";
}
void buildMap(string &path)
{
    unsigned long startTime = clock();
    try { addFolder(path); }
    catch (...) { std::cout << "Something went wrong. Try another hash function\n"; return; }
    std::cout << "Folder: " << path << '\n';
    bool rebuiltAlready = false;
    std::cout << "Average number of elements in a row: " << hm->avg() << '%' << '\n';
    std::cout << "Max elements in row: " << hm->getAlpha() << '\n';
    unsigned long endTime = clock();
    std::cout << "It took: " << endTime - startTime << "ms" << '\n';
    if (hm->avg() > hm->getMAX_ALPHA() && !rebuiltAlready)
    {
        startTime = clock();
        hm->alphaInRow = 0;
        hm->hashCount = 0;
        hm->hashType = !hm->hashType;
        rebuild(hm->hashType, path);
        std::cout << "Average number of elements in a row: " << hm->avg() << '%' << '\n';
        std::cout << "Max elements in row: " << hm->getAlpha() << '\n';
        rebuiltAlready = true;
        endTime = clock();
        std::cout << "It took: " << endTime - startTime << "ms" << '\n';
    }
    std::cout << hm->Count() << " elements.\n";
}
void act(int c)
{
    string name;
    std::cout << "Input path: ";
    std::cin >> name;
    if (c == 0)
    {
        hm->~HashMap();
        hm = new HashMap();
        buildMap(name);
    }
    else
        if (c == 1)
        {
            hm->~HashMap();
            hm = new HashMap();
            hm->hashType = !hm->hashType;
            rebuild(hm->hashType, name);
        }
        else
            if (c == 2)hm->printMap(name);
}
int main(int argc, char** argv)
{
    setlocale(LC_ALL, "RUSSIAN");
    string path;
    bool running = true;
    if (argc != 1)TABLE_SIZE = atoi(argv[1]);
    hm = new HashMap();
    do
    {
        std::cin.clear();
        std::cout << "You can run this app with TABLE_SIZE as an argument.\n";
        std::cout << "1-build table with RHash, 2-build table with md5, 3-rebuild table with different hash, 4-print table to file, 5-print table(keys only) to console, 6-set ALPHA_MAX, other value-exit\n";
        int choice = 0;
        std::cin >> choice;
        switch (choice)
        {
        case 1:hm->hashType = 0; act(0); break;
        case 2:hm->hashType = 1; act(0); break;
        case 3:act(1); break;
        case 4:act(2); break;
        case 5:hm->printMap(); break;
        case 6:
        {
            std::cout << "ALPHA_MAX = ";
            double d = 100;
            std::cin >> d;
            hm->setMAX_ALPHA(d);
        }; break;
        default: running = false; break;
        }
        system("pause");
        system("cls");
    } while (running);
    return 0;
}