//
// Created by Kendrick on 2021/4/23.
//

#include "diskmanager.h"

diskmanager::diskmanager() {
    diskLevel *level0 = new diskLevel(0);
    levels.push_back(level0);
}

diskmanager::~diskmanager() {
    for(int i = 0;i < levels.size(); ++i){
        delete levels[i];
        levels[i] = nullptr;
    }
}

void diskmanager::insert(SSTable *table) {
    levels[0]->fileMap.emplace(map_key(table->Header.timestamp,0),table);


    int i = 0;
    int compactNum = 0;

    if(levels[0]->fileMap.size() > 2){
        compactNum = levels[0]->fileMap.size();
    }
    while(compactNum > 0){
        if(levels.size() == i + 1){
            levels[i]->isLastLevel = false;
            diskLevel *newLevel = new diskLevel(i + 1);
            levels.push_back(newLevel);
        }

        compactNum = levels[i + 1]->compaction(compactNum,levels[i]);
        ++i;
    }
}

std::string diskmanager::get(uint64_t key) {
    size_t sizeOfLevels = levels.size();
    std::string value;
    uint64_t timestamp = 0;

    for(int i = 0;i < sizeOfLevels; ++i){
        uint64_t tmpNumber = 0;
        std::string tmpStr = levels[i]->get(key,tmpNumber);
        if(tmpNumber > timestamp){
            value = tmpStr;
            timestamp = tmpNumber;
        }
    }

    return value;
}

void diskmanager::reset() {
    for(int i = 0;i < levels.size(); ++i){
        levels[i]->reset();
        utils::rmdir((char*)levels[i]->levelPath.data());
        delete levels[i];
        levels[i] = nullptr;
    }
    levels.clear();

    TIMESTAMP = 1;
    diskLevel *level0 = new diskLevel(0);
    levels.push_back(level0);
}
