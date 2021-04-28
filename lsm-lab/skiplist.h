#pragma

#ifndef SKIP_LIST_H
#define SKIP_LIST_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>
#include <time.h>
#include "global.h"

struct node
{
    node *pre,*next,*down;
    std::pair<uint64_t,std::string> data;

    node(node *pre,node *next,node *down,
    std::pair<uint64_t,std::string> data): 
        pre(pre),next(next),down(down),data(data){}

    node(node *pre,node *next,node *down,
    uint64_t key,std::string value):
        pre(pre),next(next),down(down),
        data(std::pair<uint64_t,std::string>(key,value)){}

    node():pre(nullptr),next(nullptr),down(nullptr) {}

    uint64_t key(){
        return data.first;
    }

    std::string value(){
        return data.second;
    }
};


class SkipList {

private:
    node *head;
    node* find(uint64_t key);
    size_t listSize = 0;
    uint32_t fileSize = BASESIZE;

public:
    SkipList();
    ~SkipList();
    size_t size();
    std::string get(uint64_t key);
    bool put(uint64_t key,std::string value);
    bool del(uint64_t key);
    void reset();
    void getAll(std::vector<PAIR> &vec);
    void updateFileSize(const std::string *newValue,const std::string *oldValue);

    void show();
};

#endif //SKIP_LIST_H