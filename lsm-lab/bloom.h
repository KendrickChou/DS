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

    void bytes_to_bitset(char *bytes);

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

/**
 * bitset_to_bytes: convert bitset to char array.
 * bit to bit,rather than bit to byte
 */
inline char* bloom::bitset_to_bytes(){
    size_t CHAR_SIZE = BLOOM_SIZE >> 3;
    char *bloom_in_char = new char [CHAR_SIZE];
    memset(bloom_in_char,0,CHAR_SIZE * sizeof(char));

    for(int i = 0; i < BLOOM_SIZE;){
        bloom_in_char[i >> 3] |= ((filter[i]) << (i & 7));
        ++i;
        bloom_in_char[i >> 3] |= ((filter[i]) << (i & 7));
        ++i;
        bloom_in_char[i >> 3] |= ((filter[i]) << (i & 7));
        ++i;
        bloom_in_char[i >> 3] |= ((filter[i]) << (i & 7));
        ++i;
        bloom_in_char[i >> 3] |= ((filter[i]) << (i & 7));
        ++i;
        bloom_in_char[i >> 3] |= ((filter[i]) << (i & 7));
        ++i;
        bloom_in_char[i >> 3] |= ((filter[i]) << (i & 7));
        ++i;
        bloom_in_char[i >> 3] |= ((filter[i]) << (i & 7));
        ++i;
    }

    return bloom_in_char;
}

/**
 * bitset_to_bytes: convert char array to bitset.
 * bit to bit,rather than byte to bit.
 */
inline void bloom::bytes_to_bitset(char* bytes){
    for(int i = 0; i < BLOOM_SIZE; ++i){
        this->filter[i] = (bytes[i >> 3] >> (i & 7)) & 1;
    }
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

/**
 * mayContain: Check if a key value is possible
 */
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

/**
 * override operator "<<"
 * (for test)
 */
inline std::ostream & operator <<(std::ostream & os, bloom &Filter){
    os << Filter.filter;
    return os;
}


#endif  //BLOOM_H