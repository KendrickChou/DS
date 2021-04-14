#ifndef SKIP_LIST_H
#define SKIP_LIST_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <string>

struct node
{
    node *pre,*next,*down;
    std::pair<int64_t,std::string> data;

    node(node *pre,node *next,node *down,
    std::pair<int64_t,std::string> data): 
        pre(pre),next(next),down(down),data(data){}

    node(node *pre,node *next,node *down,
    int64_t key,std::string value):
        pre(pre),next(next),down(down),
        data(std::pair<int64_t,std::string>(key,value)){}

    node():pre(nullptr),next(nullptr),down(nullptr) {}

    int64_t key(){
        return data.first;
    }

    std::string value(){
        return data.second;
    }
};


class SkipList {

private:
    node *head;

public:
    SkipList();
    ~SkipList();
    size_t size();
    std::string get(int64_t key);
    void put(int64_t key,std::string value);
    bool del(int64_t key);
    void reset();
    node* find(int64_t key);

    void show();
};

#endif //SKIP_LIST_H