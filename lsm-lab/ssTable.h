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
    bloom *filter;
    std::fstream file;

    void locateValuePos(uint64_t key, 
        uint32_t &beginPos,uint32_t &len);

public:
    header Header;
    std::string filePath;
    std::vector<std::pair<uint64_t,uint32_t>> index;

    SSTable(std::string filePath,
        std::vector<PAIR> pairVec);
    SSTable();
    ~SSTable();

    bool isContain(uint64_t key);
    std::string get(uint64_t key);
    void del(uint64_t key);
    void reset();

    std::string getByOffset(uint32_t begin,uint32_t end);
};

#endif  //SSTable_H