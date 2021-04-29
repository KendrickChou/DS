#include "skiplist.h"

SkipList::SkipList(){
    srand(time(NULL));
    head = new node;
}

SkipList::~SkipList (){
    reset();
    delete head;
}

/**
 * updateFileSize:
 * if only newValue == NULL, means delete a node from list
 * if only oldValue == NULL, means add a node from list
 * if both != NULL, means update a node from list
*/
void SkipList::updateFileSize(const std::string *newValue,const std::string *oldValue){
    if(!oldValue){
        fileSize += newValue->size() + INDEXSIZE;
    }
    else if(!newValue){
        fileSize -= (oldValue->size() + INDEXSIZE);
        return;
    }
    else{
        fileSize += (newValue->size() - oldValue->size());
    }
    return;
}

/**
 * find: find node by key.
 * if failed, return nullptr.
 */
node* SkipList::find(uint64_t key){
    node *iter = head;
    while(iter){
        while(iter->next && iter->next->data.first < key){
            iter = iter->next;
        }
        if(iter->next && iter->next->data.first == key) return iter->next;
        iter = iter->down;
    }
    return nullptr;
}

/**
 * get: get value by key.
 * if failed, return an empty string.
 */
void SkipList::get(uint64_t key, std::string &value){
    node *target = this->find(key);
    if(target){
        value = target->data.second;
        return;
    }
    return;
}

/**
 * put: put a KV to skiplist
 * if failed, return false.
 */
bool SkipList::put(uint64_t key,const std::string &value){
    std::vector<node *> pathList;   //store search path
    node *iter;

    //if exist, override it
    iter = find(key);
    if(iter){
        updateFileSize(&value,&iter->data.second);
        if(fileSize > MAXSIZE) return false;
        while(iter){
            iter->data.second = value;
            iter = iter->down;
        }
        return true;
    }

    updateFileSize(&value,nullptr);
    if(fileSize > MAXSIZE) return false;

    iter = head;
    //get path

    while(iter){
        while(iter->next && iter->next->data.first < key){
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
    return true;
}

/**
 * del: del value by key.
 * if failed, return false.
 */
bool SkipList::del(uint64_t key){
    node *iter = this->find(key);

    if(!iter) return false;
    updateFileSize(nullptr,&iter->data.second);
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

/**
 * reset: reset skiplist.
 * clear all nodes
 */
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
    fileSize = BASESIZE;
}

/**
 * show: print details of skiplist to screen.
 */
void SkipList::show(){
    node *iter = head;
    node *headIter = head;
    while(iter) {
        while (iter->next) {
            iter = iter->next;
            std::cout << iter->data.first << " " << iter->data.second << "|";
        }
        std::cout << '\n';
        iter = headIter->down;
        headIter = iter;
    }
}

/**
 * getALL: get all KV.
 * All KVs store in a vector
 */
void SkipList::getAll(std::vector<PAIR> &vec){

    if(!head->next) {
        return;
    }

    node *iter = head;

    while(iter->down){
        iter = iter->down;
    }

    while(iter->next){
        vec.push_back(iter->next->data);
        iter = iter->next;
    }

}