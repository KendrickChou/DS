#include "diskLevel.h"

diskLevel::diskLevel(int level,bool restore):level(level), MAXNumber(pow(2,level + 1)) {

}

diskLevel::diskLevel(int level): level(level), MAXNumber(pow(2,level + 1)) {
    levelPath = "./data/level" + std::to_string(level);
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

    std::map<map_key ,SSTable *> compactSSTable;
    matchScope(compactNum,maxtimestamp,upLevel,compactSSTable);  //sort by timestamp increase
    std::vector<PAIR> compactVec[compactSSTable.size()];
    std::vector<PAIR> newVec;
    uint32_t vecSize = BASESIZE;

    convert2vector(compactSSTable,compactVec);

    for(auto iter = compactSSTable.begin();iter != compactSSTable.end(); ++iter){
        utils::rmfile((char*)iter->second->filePath.c_str());
        delete iter->second;
    }

    std::priority_queue<que_key,
            std::vector<que_key>,LESS> que;

    for(int i = compactSSTable.size() - 1;i >= 0 ; --i){
        que.emplace(compactVec[i].back(),i);
        compactVec[i].pop_back();
    }

    que_key oldVal(PAIR(4294967295,""),-1),newVal(PAIR(4294967295,""),-1);
    while(true) {
        if(que.empty()){
            break;
        }

        newVal = que.top();
        que.pop();

        int curVec = newVal.second;
        if(newVal.first.first != oldVal.first.first){
            push_back(vecSize,maxtimestamp,newVec,newVal.first);
            oldVal = newVal;
        }

        if(!compactVec[curVec].empty()){
            que.emplace(compactVec[curVec].back(),curVec);
            compactVec[curVec].pop_back();
        }
    }

    if(!newVec.empty()){
        std::string filePath = levelPath + "/" + std::to_string(maxtimestamp) + "-" + std::to_string(newVec[0].first) + ".sst";
        SSTable *newTable = new SSTable(filePath,maxtimestamp,newVec);
        fileMap.emplace(map_key(maxtimestamp,newTable->Header.minKey),newTable);
    }

    if(fileMap.size() > MAXNumber)
        return fileMap.size() - MAXNumber;
        
    return 0;
}

void diskLevel::matchScope(int compactNum, uint64_t &maxTimeStamp, diskLevel *upLevel,std::map<map_key ,SSTable*> &res) {
    auto iter = this->fileMap.begin();

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
            if((iter->second->Header.minKey <= minKey && iter->second->Header.maxKey >= minKey) ||
                    (iter->second->Header.maxKey >= maxKey && iter->second->Header.minKey <= maxKey)){
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
}

void diskLevel::convert2vector(const std::map<map_key ,SSTable *> &sets,
                               std::vector<std::pair<uint64_t, std::string>> *vecs) {
    if(sets.empty()) return;


    size_t sizeOfsets = sets.size();
    auto iter = sets.begin();

    for(int i = 0; i < sizeOfsets; ++i){
        iter->second->file.open(iter->second->filePath,std::ios_base::in|std::ios_base::binary);
        int pos = iter->second->Header.numOfkey - 1;
        uint64_t key = 0;
        uint32_t offset = 0;
        uint32_t nextOffset = 0;
        std::string value;

        key = iter->second->index[pos].first;
        offset = iter->second->index[pos].second;
        iter->second->getByOffset(value,offset,0);
        vecs[i].emplace_back(key,value);

        nextOffset = offset;
        while(--pos >= 0){
            key = iter->second->index[pos].first;
            offset = iter->second->index[pos].second;
            iter->second->getByOffset(value,offset,nextOffset);
            vecs[i].emplace_back(key,value);
            nextOffset = offset;
        }

        iter->second->file.close();
        iter->second->file.clear();
        ++iter;
    }
    }

void diskLevel::push_back(uint32_t &size, uint64_t timestamp,std::vector<PAIR> &vec, PAIR &KV) {
    if(this->isLastLevel && KV.second == "~DELETED~"){
        return;
    }

    if((size += INDEXSIZE + KV.second.size()) <= MAXSIZE){
        vec.push_back(KV);
    }
    else{
        std::string fileName = "/" + std::to_string(timestamp) + "-" + std::to_string(vec[0].first) + ".sst";
        SSTable *newTable = new SSTable (levelPath + fileName,timestamp,vec);
        this->fileMap.emplace(map_key (newTable->Header.timestamp,newTable->Header.minKey),newTable);
        vec.clear();
        vec.push_back(KV);
        size = BASESIZE + INDEXSIZE + KV.second.size();
    }
}

void diskLevel::get(uint64_t key,std::string &value) {
    auto iter = fileMap.rbegin();

    for(;iter != fileMap.rend();++iter){
        iter->second->get(key,value);
        if(!value.empty()){
            return;
        }
    }

    return;

}

void diskLevel::reset() {
    for(auto iter = fileMap.begin(); iter != fileMap.end(); ++iter){
        utils::rmfile((char *)iter->second->filePath.data());
    }

    rmdir((char *)levelPath.c_str());
}

void diskLevel::insert(const std::vector<PAIR> &vec,uint64_t timestamp) {
    std::string filePath = "./data/level0/" + std::to_string(timestamp) + "-" + std::to_string(vec[0].first) + ".sst";
    SSTable *newTable = new SSTable (filePath,timestamp,vec);

    this->fileMap.emplace(map_key(newTable->Header.timestamp,newTable->Header.minKey),newTable);
}

int diskLevel::restoreLevel(const std::string &path){
    std::vector<std::string> fileName;

    utils::scanDir(path,fileName);

    for(auto name : fileName){
        SSTable *newTable = new SSTable(path + "/" + name);
        fileMap.emplace(map_key(newTable->Header.timestamp,
                    newTable->Header.minKey),newTable);
    }

    if(fileMap.size() > MAXSIZE){
        return fileMap.size() - MAXSIZE;
    }
    return 0;
    
}