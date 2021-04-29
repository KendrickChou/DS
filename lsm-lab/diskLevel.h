#pragma

#ifndef DISKLEVEL_H
#define DISKLEVEL_H

#include <iostream>
#include <cmath>
#include <map>
#include <set>
#include <queue>
#include "ssTable.h"

typedef std::pair<PAIR,int> que_key;

struct LESS{
    bool operator() (const que_key &kv1,const que_key &kv2){
        return ((kv1.first.first > kv2.first.first) ||
                (kv1.first.first == kv2.first.first && kv1.second < kv2.second));
    }
};

class diskLevel{
private:
    int level;

    void matchScope(int compactNum, uint64_t &maxTimeStamp, diskLevel *upLevel,std::map<map_key ,SSTable*> &res);
    void convert2vector(const std::map<map_key ,SSTable *> &sets,
                        std::vector<std::pair<uint64_t, std::string>> *vecs);
    void push_back(uint32_t &size,uint64_t timestamp,std::vector<PAIR> &vec, PAIR &KV);

public:
    const int MAXNumber;
    std::string levelPath;
    bool isLastLevel = true;

    std::map<map_key ,SSTable* > fileMap;

    diskLevel(int level);
    diskLevel(int level,bool restore);
    ~diskLevel();

    void insert(const std::vector<PAIR> &vec,uint64_t timestamp);
    int compaction(int compactNum,diskLevel *upLevel);
    void reset();
    void get(uint64_t key,std::string &value);

    int restoreLevel(const std::string &path);
};

#endif  //DISKLEVEL_H
