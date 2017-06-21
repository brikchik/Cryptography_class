#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include "../shared/rh.h"
#include "../shared/md5.h"
#include <vector>
//class HashEntry {
//private:
//    unsigned char* key=NULL;
//    char* value=NULL;
//public:
//    HashEntry(unsigned char* key, char* value) {
//        this->key = key;
//        this->value = value;
//    }
//    HashEntry() {
//        key = NULL;
//        value = NULL;
//    }
//    unsigned char* getKey() {
//        return key;
//    }
//    char* getValue() {
//        return value;
//    }
//};
//const int TABLE_SIZE = 128;
//const int ROW_SIZE = 128;
//class HashContent
//{
//private:
//    unsigned char* hashedKey;
//    HashEntry *row;
//    int count = 0;
//    unsigned char* hash(unsigned char* key) { return (unsigned char*)strlen((char*)key); }
//public:
//    HashContent(unsigned char* hKey)
//    {
//        this->hashedKey = hash(hKey);
//    }
//    HashContent() {
//        row = new HashEntry[ROW_SIZE];
//    }
//    int Count() { return count; }
//    char* get(unsigned char* key) {
//        int foundIndex = -1;
//        for (int i = 0; i < count; i++)
//            if (row[i].getKey() == key)foundIndex = i;
//        if (foundIndex != -1)return row[foundIndex].getValue();
//        else return NULL;
//    }
//    void put(unsigned char* key, char* value) {
//        row = new HashEntry();
//        bool found = false;
//        for (int i = 0; i < count; i++)
//            if (row[i].getKey() == key)found = true;//проверка на наличие этого файла
//        if (!found)
//        {
//            row[count] = HashEntry(key, value);
//            count++;
//            this->hashedKey = hash(key);
//        }
//    }
//    unsigned char* getHashedKey() { return hashedKey; }
//};
//class HashMap {
//private:
//    HashContent *HashContents;
//    int count=0;
//    unsigned char* hash(unsigned char* key) { return (unsigned char*)strlen((char*)key); }
//public:
//    HashMap() {
//        HashContents = new HashContent[TABLE_SIZE];
//    }
//    void put(unsigned char* key, char* value)
//    {
//        bool success = false;
//        if (HashContents[0].Count() == 0)HashContents[0] = HashContent();
//        for (int i = 0; i < count; i++)
//            if (HashContents[i].getHashedKey() == hash(key))HashContents[i].put(key, value);
//        if (!success)
//        {
//            HashContents[count] = HashContent(key);
//            HashContents[count].put(key, value);
//            count++;
//        }
//    }
//    char* get(unsigned char* key)
//    {
//        for (int i = 0; i < count; i++)
//        {
//            if (HashContents[i].getHashedKey() == hash(key))
//                return HashContents[i].get(key);
//        }
//        return "";
//    }
//};





//http://www.algolist.net/Data_structures/Hash_table/Chaining
class filePar
{
public:
    char* name;
    int attr;
};
class LinkedHashEntry {
private:
    char* key;
    filePar value;
    LinkedHashEntry *next;
public:
    LinkedHashEntry(char* key, filePar value) {
        this->key = key;
        this->value = value;
        this->next = NULL;
    }

    char* getKey() {
        return key;
    }

    filePar getValue() {
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

const int TABLE_SIZE = 1280;

class HashMap {
private:
    rh rhclass;
    int Hash(char* key) { 
        int result = 0;
        std::vector<unsigned char> temp;
        std::vector<unsigned char> temp2;
        for (int i = 0; i < strlen(key); i++)temp.push_back(key[i]);
        temp2 = rhclass.Hash(temp);
        std::cout << temp2.at(0);///////////////////////////////////////////////key
        return result % 128; }
    LinkedHashEntry **table;
public:
    HashMap() {
        table = new LinkedHashEntry*[TABLE_SIZE];
        for (int i = 0; i < TABLE_SIZE; i++)
            table[i] = NULL;
    }
    filePar get(char* key) {
        int hash = Hash(key);
        std::cout << '_' << hash << '_';
        if (table[hash] == NULL)
            return filePar();// emptyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy
        else {
            LinkedHashEntry *entry = table[hash];
            while (entry != NULL && entry->getKey() != key)
                entry = entry->getNext();
            if (entry == NULL)
                return filePar();// emptyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyy
            else
                return entry->getValue();
        }
    }

    void put(char* key, filePar value) {

        int hash = Hash(key);
        if (table[hash] == NULL)
            table[hash] = new LinkedHashEntry(key, value);
        else {
            LinkedHashEntry *entry = table[hash];
            while (entry->getNext() != NULL)
                entry = entry->getNext();
            if (entry->getKey() == key)
                entry->setValue(value);
            else
                entry->setNext(new LinkedHashEntry(key, value));
        }
    }
    void remove(char* key) {
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


int main()
{
    std::cin.clear();
    filePar fp;
    fp.name = "fp1";
    fp.attr = 9;
    filePar fp2;
    fp2.name = "21";
    fp2.attr = 29;
    HashMap hm;
    
    hm.put("f1", fp);
    hm.put("gg", fp2);
    std::cout << hm.get("f1").name<<'\n';
    std::cout << hm.get("gg").name;
    //std::cout<<hm.get(12);
    system("pause");
    return 0;
}