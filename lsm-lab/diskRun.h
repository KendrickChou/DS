#pragma

#ifndef DISKRUN_H
#define DISKRUN_H

#include <vector>
#include <string>
#include <utility>
#include <fstream>
#include <iostream>
#include "bloom.h"
#include "skiplist.h"
#include "utils.h"

const uint64_t MAXSIZE = 2097152; //byte
const uint32_t BASESIZE = 10272; //byte
const uint32_t HEADERSIZE = 32; //byte
const uint32_t INDEXSIZE = 12;  //byte

typedef std::pair<uint64_t,uint32_t> PAIR;

class DiskRun {
private:
    std::string header;
    bloom *filter;
    std::fstream file;
    size_t sizeOfIndex;
    uint64_t minKey,maxKey;

    void locateValuePos(uint64_t key, 
        uint32_t &beginPos,uint32_t &len);

public:
    DiskRun(std::string fileName,
        std::string header,
        size_t sizeofPairVec,
        std::vector<std::pair<uint64_t,std::string>> pairVec);
    DiskRun();
    ~DiskRun();

    bool isContain(uint64_t key);
    std::string get(uint64_t key);
    void del(uint64_t key);
    void reset();
};

#endif  //DISKRUN_H