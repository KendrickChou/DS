//
// Created by Kendrick on 2021/4/23.
//

#ifndef LSM_TREE_DISKMANAGER_H
#define LSM_TREE_DISKMANAGER_H

#include <iostream>
#include <vector>
#include "diskLevel.h"

class diskmanager {
private:
    std::vector<diskLevel*> levels;

public:
    diskmanager();
    ~diskmanager();

    void insert(SSTable *table);
    std::string get(uint64_t key);
    void reset();
};


#endif //LSM_TREE_DISKMANAGER_H
