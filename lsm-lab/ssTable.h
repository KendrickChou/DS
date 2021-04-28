#pragma

#ifndef SSTable_H
#define SSTable_H

#include <vector>
#include <string>
#include <utility>
#include <fstream>
#include <iostream>
#include "bloom.h"
#include "skiplist.h"
#include "utils.h"
#include "global.h"

class SSTable {
private:
    bloom filter;

    void locateValuePos(uint64_t key, 
        uint32_t &beginPos,uint32_t &len);
    bool isContain(uint64_t key);

public:
    header Header;
    std::string filePath;
    std::fstream file;
    std::vector<std::pair<uint64_t,uint32_t>> index;

    SSTable(std::string filePath,
            uint64_t timestamp,
        std::vector<PAIR> pairVec);
    SSTable();
    ~SSTable();

    std::string get(uint64_t key);

    std::string getByOffset(uint32_t begin,uint32_t end);
};

#endif  //SSTable_H