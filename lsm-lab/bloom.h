#pragma

#ifndef BLOOM_H
#define BLOOM_H

#include <iostream>
#include <bitset>
#include <vector>
#include <cstring>
#include "MurmurHash3.h"

const static size_t SIZE = 5;

class bloom
{
private:
    std::bitset<SIZE> filter;

    unsigned int* hashKey(uint64_t key);

public:
    bloom();
    ~bloom();

    void addKey(uint64_t key);
    bool mayContain(uint64_t key);

    friend std::ostream & operator <<(std::ostream & os, const bloom &Filter);
    friend std::istream & operator >>(std::istream & is, bloom &Filter);
};

bloom::bloom()
{
}

bloom::~bloom()
{
}

unsigned int* bloom::hashKey(uint64_t key){
    unsigned int *hash = new unsigned int [4];
    memset(hash,0,sizeof(unsigned int)*4);

    MurmurHash3_x64_128(&key,sizeof(key),1,hash);

    return hash;
}

void bloom::addKey(uint64_t key){
    unsigned int *hash = hashKey(key);

    for(int i = 0;i < 4;++i){
        this->filter[(hash[i] % SIZE)] = true;
    }

    delete hash;
    return;
}

bool bloom::mayContain(uint64_t key){
    unsigned int *hash = hashKey(key);

    for(int i = 0;i < 4;++i){
        if(!this->filter[(hash[i] % SIZE)]){
            return false;
        }
    }

    delete hash;
    return true;
}

std::ostream & operator <<(std::ostream & os, const bloom &Filter){
    os << Filter.filter;
    return os;
}

std::istream & operator >>(std::istream & is,bloom &Filter){
    char filter_string[SIZE + 1];
    is.get(filter_string,SIZE + 1);
    std::bitset<SIZE> newFilter{std::string(filter_string)};
    Filter.filter.reset();
    Filter.filter |= newFilter;
    return is;
}


#endif  //BLOOM_H