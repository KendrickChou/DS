#pragma

#ifndef SKIP_LIST_H
#define SKIP_LIST_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>

const long long INDEXSIZE = 96;
const long long BASESIZE = 10496; //HEADERSIZE + FILTERSIZE

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
    long long fileSize = BASESIZE; //bit
    void updateFileSize(const std::string *newValue,const std::string *oldValue);

public:
    SkipList();
    ~SkipList();
    size_t size();
    std::string get(uint64_t key);
    void put(uint64_t key,std::string value);
    bool del(uint64_t key);
    void reset();
    std::vector<std::pair<uint64_t,std::string>>* getAll();

    void show();
};

#endif //SKIP_LIST_H