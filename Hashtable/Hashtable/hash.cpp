#include <stdio.h>
#include "../../7_Hash/7_Hash/rh.h"
class HashEntry {
private:
    unsigned char* key;
    char* value;
public:
    HashEntry(unsigned char key[12], char* value) {
        this->key = key;
        this->value = value;
    }

    unsigned char* getKey() {
        return key;
    }

    char* getValue() {
        return value;
    }
};
const int TABLE_SIZE = 128;

class HashMap {
private:
    HashEntry **table;
public:
    HashMap() {
        table = new HashEntry*[TABLE_SIZE];
        for (int i = 0; i < TABLE_SIZE; i++)
            table[i] = NULL;
    }

    char* get(int key) {
        unsigned char* hash = Hash(std::vector<unsigned char>(key),false).data();
        while (table[hash] != NULL && table[hash]->getKey() != key)
            hash = (hash + 1) % TABLE_SIZE;
        if (table[hash] == NULL)
            return "";
        else
            return table[hash]->getValue();
    }

    void put(int key, char* value) {
        int hash = (key % TABLE_SIZE);
        while (table[hash] != NULL && table[hash]->getKey() != key)
            hash = (hash + 1) % TABLE_SIZE;
        if (table[hash] != NULL)
            delete table[hash];
        table[hash] = new HashEntry(key, value);
    }

    ~HashMap() {
        for (int i = 0; i < TABLE_SIZE; i++)
            if (table[i] != NULL)
                delete table[i];
        delete[] table;
    }
};