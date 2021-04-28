//
// Created by Kendrick on 2021/4/23.
//

#include "diskcontroller.h"

DiskController::DiskController() {
    diskLevel *level0 = new diskLevel(0);
    levels.push_back(level0);
}

DiskController::~DiskController() {
    for(int i = 0;i < levels.size(); ++i){
        delete levels[i];
        levels[i] = nullptr;
    }

}

void DiskController::insert(const std::vector<PAIR> &vec) {
    levels[0]->insert(vec,TIMESTAMP++);

    int i = 0;
    int compactNum = 0;

    if(levels[0]->fileMap.size() > 2) {
        compactNum = levels[0]->fileMap.size();
        while (compactNum > 0) {
            if (levels.size() == i + 1) {
                levels[i]->isLastLevel = false;
                diskLevel *newLevel = new diskLevel(i + 1);
                levels.push_back(newLevel);
            }

            compactNum = levels[i + 1]->compaction(compactNum, levels[i]);
            ++i;
        }
    }
}

void DiskController::get(uint64_t key,std::string &value) {
    size_t sizeOfLevels = levels.size();
    uint64_t timestamp = 0;

    for(int i = 0;i < sizeOfLevels; ++i){
        levels[i]->get(key,value);
        if(!value.empty()){
            return;
        }
    }
}

void DiskController::reset() {
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
