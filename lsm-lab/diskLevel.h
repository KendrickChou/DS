#pragma

#ifndef DISKLEVEL_H
#define DISKLEVEL_H

#include <iostream>
#include <cmath>
#include <map>
#include <set>
#include <queue>
#include "ssTable.h"

struct less{
    bool operator() (std::pair<uint64_t,std::string> kv1,
                     std::pair<uint64_t,std::string> kv2){
        return kv1.first < kv2.first;
    }
};

class diskLevel{
private:
    int level;

    std::map<map_key ,SSTable*> matchScope(int compactNum, uint64_t &maxTimeStamp, diskLevel *upLevel);
    void convert2vector(std::map<map_key ,SSTable *> sets,
                        std::vector<std::pair<uint64_t, std::string>> *vecs);
    void push_back(uint32_t &size,uint64_t timestamp,uint64_t &secondaryKey,std::vector<PAIR> &vec, PAIR KV);

public:
    const int MAXNumber;
    std::string levelPath;
    bool isLastLevel = true;

    std::map<map_key ,SSTable* > fileMap;

    diskLevel(int level);
    ~diskLevel();

    int compaction(int compactNum,diskLevel *upLevel);
    void reset();
    std::string get(uint64_t key,uint64_t &timestamp);
};

#endif  //DISKLEVEL_H