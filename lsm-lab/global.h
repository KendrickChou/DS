//
// Created by Kendrick on 2021/4/20.
//

#ifndef LSM_TREE_GLOBAL_H
#define LSM_TREE_GLOBAL_H

#include <iostream>

typedef std::pair<uint64_t,std::string> PAIR;
typedef std::pair<uint64_t,uint64_t> map_key;   // first: timestamp, second: default 0, if there is same timestamp, then sequential grow


// The following constants are in bytes

const uint32_t  HEADERSIZE = 32; //timestamp + pairNumber + minKey, maxKey
const uint32_t  INDEXSIZE = 12; //key + offset
const uint32_t  OFFSETSIZE = 4;
const uint32_t  BASESIZE = 10272; //header + bloomfilter
const uint32_t  MAXSIZE = 2097152;

// The following constants are in bits
const size_t BLOOM_SIZE = 81920;


struct header{
    uint64_t timestamp = 0;
    uint64_t numOfkey = 0;
    uint64_t minKey = 0;
    uint64_t maxKey = 0;
};

#endif //LSM_TREE_GLOBAL_H
