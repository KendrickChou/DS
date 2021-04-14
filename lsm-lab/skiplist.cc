#include "skiplist.h"

SkipList::SkipList(){
    head = new node;
}

SkipList::~SkipList (){
    reset();
    delete head;
}

size_t SkipList::size(){
    node *iter = head;
    while(iter->down){
        iter = iter->down;
    }
    size_t res = 0;
    iter = iter->next;
    while(iter){
        ++res;
        iter = iter->next;
    }
    return res;
}

node* SkipList::find(int64_t key){
    node *iter = head;
    while(iter){
        while(iter->next && iter->next->key() < key){
            iter = iter->next;
        }
        if(iter->next && iter->next->key() == key) return iter->next;
        iter = iter->down;
    }
    return nullptr;
}

std::string SkipList::get(int64_t key){
    node *target = this->find(key);
    if(target) return target->value();
    return "";
}

void SkipList::put(int64_t key,std::string value){
    std::vector<node *> pathList;   //store search path
    node *iter;

    //if exist, override it
    iter = find(key);
    if(iter){
        while(iter){
            iter->data.second = value;
            iter = iter->down;
        }
        return;
    }

    iter = head;
    //get path

    while(iter){
        while(iter->next && iter->next->key() < key){
            iter = iter->next;
        }
        pathList.push_back(iter);
        iter = iter->down;
    }

    //insert a null node to insert more convenient
    node *tail = new node;
    pathList.push_back(tail);
    bool continueInsert = true;

    for(int i = pathList.size() - 2;i >= 0;--i){
        if(!continueInsert){
            break;
        }
        node *putNode = new node(nullptr,nullptr,nullptr, key,value);
        continueInsert = (rand()&1);    //50% continue input
        iter = pathList[i];
        node *nextNode = iter->next;
        iter->next = putNode;
        putNode->pre = iter;
        if(nextNode){
            nextNode->pre = putNode;
            putNode->next = nextNode;
        }
        putNode->down = pathList[i + 1]->next;
    }

    // check if add a new level

    if(continueInsert){
        node *newHead = new node;
        node *putNode = new node(nullptr,nullptr,nullptr,key,value);
        newHead->down = head;
        newHead->next = putNode;
        putNode->pre = newHead;
        putNode->down = pathList[0]->next;
        head = newHead;
    }

    delete tail;
    return;
}

bool SkipList::del(int64_t key){
    node *iter = this->find(key);

    if(!iter) return false;
    node *delNode,*preNode,*nextNode;
    while(iter){
        delNode = iter;
        iter = iter->down;
        preNode = delNode->pre;
        nextNode = delNode->next;
        preNode->next = nextNode;
        if(nextNode) nextNode->pre = preNode;
        delete delNode;
        if(!head->next && head->down){
            delNode = head;
            head = head->down;
            delete delNode;
        }
    }
    return true;
}

void SkipList::reset(){
    while(head){
        node *iter = head->next;
        while(iter){
            node *delNode = iter;
            iter = iter->next;
            delete delNode;
        }
        iter = head;
        head = head->down;
        delete iter;
    }
    head = new node;
}

void SkipList::show(){
    node *iter = head;
    node *headIter = head;
    while(iter) {
        while (iter->next) {
            iter = iter->next;
            std::cout << iter->key() << " " << iter->value() << "|";
        }
        std::cout << '\n';
        iter = headIter->down;
        headIter = iter;
    }
}