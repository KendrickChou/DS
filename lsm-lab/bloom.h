#pragma

#ifndef BLOOM_H
#define BLOOM_H

#include <iostream>
#include <bitset>
#include <vector>
#include <cstring>
#include "MurmurHash3.h"
#include "global.h"

class bloom
{
private:
    std::bitset<BLOOM_SIZE> filter;

    void hashKey(uint64_t key,unsigned int *hash);

public:
    bloom();
    ~bloom();

    void addKey(uint64_t key);
    bool mayContain(uint64_t key);
    char* bitset_to_bytes();

    friend std::ostream & operator <<(std::ostream & os, bloom &Filter);
};

inline bloom::bloom()
{
}

inline bloom::~bloom()
{
}

inline void bloom::hashKey(uint64_t key,unsigned int *hash){
    MurmurHash3_x64_128(&key,sizeof(key),1,hash);
}

inline char* bloom::bitset_to_bytes(){
    size_t CHAR_SIZE = BLOOM_SIZE >> 3;
    char *bloom_in_char = new char [CHAR_SIZE];
    memset(bloom_in_char,0,CHAR_SIZE * sizeof(char));

    for(int i = 0; i < BLOOM_SIZE; ++i){
        bloom_in_char[i >> 3] |= ((filter[i]) << (i & 7));
    }

    return bloom_in_char;
}

inline void bloom::addKey(uint64_t key){
    unsigned int *hash = new unsigned int [4];
    memset(hash,0,sizeof(hash));

    hashKey(key,hash);

    for(int i = 0;i < 4;++i){
        this->filter[(hash[i] % BLOOM_SIZE)] = true;
    }

    delete [] hash;
    return;
}

inline bool bloom::mayContain(uint64_t key){
    unsigned int *hash = new unsigned int [4];
    memset(hash,0,sizeof(hash));

    hashKey(key,hash);

    for(int i = 0;i < 4;++i){
        if(!this->filter[(hash[i] % BLOOM_SIZE)]){
            return false;
        }
    }

    delete [] hash;
    return true;
}

inline std::ostream & operator <<(std::ostream & os, bloom &Filter){
    os << Filter.filter;
    return os;
}

//inline std::istream & operator >>(std::istream & is,bloom &Filter){
//    char filter_string[BLOOM_SIZE + 1];
//    is.get(filter_string,BLOOM_SIZE + 1);
//    std::bitset<BLOOM_SIZE> newFilter{std::string(filter_string)};
//    Filter.filter.reset();
//    Filter.filter |= newFilter;
//    return is;
//}


#endif  //BLOOM_H