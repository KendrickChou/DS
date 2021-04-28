//
// Created by Kendrick on 2021/4/23.
//


#ifndef LSM_TREE_DISKCONTROLLER_H
#define LSM_TREE_DISKCONTROLLER_H

#include <iostream>
#include <vector>
#include "diskLevel.h"

class DiskController {
private:
    std::vector<diskLevel*> levels;
    uint64_t TIMESTAMP = 1;

public:
    DiskController();
    ~DiskController();

    void insert(const std::vector<PAIR> &vec);
    std::string get(uint64_t key);
    void reset();

};


#endif //LSM_TREE_DISKCONTROLLER_H
