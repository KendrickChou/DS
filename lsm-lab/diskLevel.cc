#include "diskLevel.h"

diskLevel::diskLevel(int level): level(level), MAXNumber(pow(2,level + 1)) {
    levelPath = "../level/level" + std::to_string(level);
    if(!utils::dirExists(levelPath)){
        utils::mkdir((char *)levelPath.data());
    } else{
        std::vector<std::string> fileVec;
        int fileNum = utils::scanDir(levelPath,fileVec);
        for(int i = 0; i < fileNum; ++i){
         utils::rmfile((char *)fileVec[i].data());
        }
    }
}

diskLevel::~diskLevel() {
    auto iter = fileMap.begin();
    while(iter != fileMap.end()){
        delete iter->second;
        iter->second = nullptr;
        ++iter;
    }
}

/*
 * compaction - compact two diskLevel.
 *  Return the number of overflowing SSTables, if there is no overflow,
 *  return 0
 */
int diskLevel::compaction(int compactNum, diskLevel *upLevel) {
    uint64_t maxtimestamp = 0;
    uint64_t sK = 0;    //secondary Key

    std::map<map_key ,SSTable *> compactSSTable = matchScope(compactNum,maxtimestamp,upLevel);  //sort by timestamp increase
    std::vector<PAIR> compactVec[compactSSTable.size()]; //sort every vec in decrease
    std::vector<PAIR> newVec;
    uint32_t vecSize = BASESIZE;

    convert2vector(compactSSTable,compactVec);

    for(auto iter = compactSSTable.begin();iter != compactSSTable.end(); ++iter){
        utils::rmfile((char*)iter->second->filePath.data());
    }

    std::priority_queue<PAIR,
            std::vector<PAIR>,less> que;

    for(int i = compactSSTable.size() - 1;i >= 0 ; --i){
        que.push(compactVec[i].back());
        compactVec[i].pop_back();
    }

    int emptyVec = 0,vecPos = compactSSTable.size() - 1;
    PAIR oldVal(0,""),newVal(0,"");
    bool firstEmpty[compactSSTable.size()];
    memset(firstEmpty,0,sizeof(bool) * compactSSTable.size());
    while(true) {
        if(compactVec[vecPos].empty()){
            if(!firstEmpty[vecPos]) {
                firstEmpty[vecPos] = 1;
                if(++emptyVec == compactSSTable.size())
                    break;
            }
            if(--vecPos < 0){
                vecPos = compactSSTable.size() - 1;
            }
            continue;
        }

        newVal = que.top();
        que.pop();

        if(newVal.first != oldVal.first){
            push_back(vecSize,maxtimestamp,sK,newVec,newVal);
            oldVal = newVal;
        }

        que.push(compactVec[vecPos].back());
        compactVec[vecPos].pop_back();

        if(--vecPos < 0){
            vecPos = compactSSTable.size() - 1;
        }
    }

    while(!que.empty()){
        newVal = que.top();
        que.pop();

        if(newVal.first != oldVal.first){
            push_back(vecSize,maxtimestamp,sK,newVec,newVal);
            oldVal = newVal;
        }
    }

    std::string filePath = levelPath + "/" + std::to_string(maxtimestamp) + "-" + std::to_string(sK) + ".sst";
    SSTable *newTable = new SSTable(filePath,newVec);
    --TIMESTAMP;
    newTable->Header.timestamp = maxtimestamp;
    fileMap.emplace(map_key(maxtimestamp,sK),newTable);

    if(fileMap.size() > MAXNumber)
        return fileMap.size() - MAXNumber;
    return 0;
}

std::map<map_key ,SSTable *> diskLevel::matchScope(int compactNum, uint64_t &maxTimeStamp, diskLevel *upLevel) {
    auto iter = this->fileMap.begin();
    std::map<map_key ,SSTable*> res;

    for(int i = 0;i < compactNum;++i){
        auto upiter = upLevel->fileMap.begin();
        res.emplace(upiter->first,upiter->second);
        upLevel->fileMap.erase(upiter->first);
    }

    auto resiter = res.begin();

    for(int i = 0;i < compactNum; ++i){
        uint64_t minKey = resiter->second->Header.minKey;
        uint64_t maxKey = resiter->second->Header.maxKey;

        ++resiter;
        for(;iter != fileMap.end();++iter){
            if((iter->second->Header.minKey < minKey && iter->second->Header.maxKey > minKey) ||
                    (iter->second->Header.maxKey > maxKey && iter->second->Header.minKey < maxKey)){
                if(!res.count(iter->first)){
                    res.emplace(iter->first,iter->second);
                }
            }
        }
        iter = this->fileMap.begin();
    }

    resiter = res.begin();
    for(;resiter != res.end();++resiter){
        if(resiter->first.first > maxTimeStamp)
            maxTimeStamp = resiter->first.first;
         if(this->fileMap.count(resiter->first)){
             this->fileMap.erase(resiter->first);
         }
    }

    return res;
}

void diskLevel::convert2vector(std::map<map_key ,SSTable *> sets,
                               std::vector<std::pair<uint64_t, std::string>> *vecs) {
    if(sets.empty()) return;

    size_t sizeOfsets = sets.size();
    auto iter = sets.begin();

    for(int i = 0; i < sizeOfsets; ++i){
        int pos = iter->second->Header.numOfkey - 1;
        uint64_t key = 0;
        uint32_t offset = 0;
        uint32_t nextOffset = 0;
        std::string value;

        key = iter->second->index[pos].first;
        offset = iter->second->index[pos].second;
        value = iter->second->getByOffset(offset,0);
        vecs[i].emplace_back(key,value);
        --pos;

        while(pos >= 0){
            key = iter->second->index[pos].first;
            offset = iter->second->index[pos].second;
            nextOffset = iter->second->index[pos + 1].second;
            value = iter->second->getByOffset(offset,nextOffset);

            vecs[i].emplace_back(key,value);
            --pos;
        }

        ++iter;
    }
    }

void diskLevel::push_back(uint32_t &size, uint64_t timestamp,uint64_t &secondaryKey ,std::vector<PAIR> &vec, PAIR KV) {
    if(isLastLevel && KV.second == "~DELETE~"){
        return;
    }

    if((size += INDEXSIZE + KV.second.size()) <= MAXSIZE){
        vec.push_back(KV);
    }
    else{
        std::string fileName = "/" + std::to_string(timestamp) + "-" + std::to_string(secondaryKey) + ".sst";
        SSTable *newTable = new SSTable (levelPath + fileName,vec);
        --TIMESTAMP;
        newTable->Header.timestamp = timestamp;
        this->fileMap.emplace(map_key (newTable->Header.timestamp,secondaryKey++),newTable);
        vec.clear();
        vec.push_back(KV);
        size = BASESIZE + INDEXSIZE + KV.second.size();
    }
}

std::string diskLevel::get(uint64_t key, uint64_t &timestamp) {
    std::map<map_key,SSTable *>::reverse_iterator iter = fileMap.rbegin();
    std::string value;

    for(;iter != fileMap.rend();++iter){
        value = iter->second->get(key);
        if(!value.empty()){
            timestamp = iter->first.first;
        }
        return value;
    }

    return value;

}

void diskLevel::reset() {
    for(auto iter = fileMap.begin(); iter != fileMap.end(); ++iter){
        utils::rmfile((char *)iter->second->filePath.data());
    }

    rmdir((char *)levelPath.data());
}
