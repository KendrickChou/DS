#include "diskRun.h"

DiskRun::DiskRun(){

}

DiskRun::DiskRun(std::string fileName,
    std::string header,
    size_t sizeofPairVec,
    std::vector<std::pair<uint64_t,std::string>> pairVec){
        this->filter = new bloom;
        this->header = header;
        this->sizeOfIndex = sizeofPairVec;

    
        file.open(fileName,std::ios::in|std::ios::out);
        this->file << header;
        uint32_t offset = BASESIZE + sizeofPairVec * INDEXSIZE;

        for(int i = 0;i < sizeofPairVec; ++i){
            filter->addKey(pairVec[i].first);
        }

        file << filter;

        for(int i = 0;i < sizeofPairVec;++i){
            file << pairVec[i].first << offset;
            offset += pairVec[i].second.size();
        }


        for(int i = 0;i < sizeofPairVec; ++i){
            file << pairVec[i].second;
        }

        minKey = pairVec[0].first;
        maxKey = pairVec[sizeofPairVec - 1].first;

        delete this->filter;
        this->filter = nullptr;
    }

DiskRun::~DiskRun(){
    if(!filter) delete filter;
}

/* input a key, find the corresponding value.
 * if exist, return beginPos and len
 * if the key is the last one. return beginPos and len = -1
 * if not, beginPos = 0, len = 0;
 */
void DiskRun::locateValuePos(uint64_t key, uint32_t &beginPos, uint32_t &len){
    uint64_t keyIter = 0;
    uint32_t indexIter = 0;

    for(int i = 0;i < sizeOfIndex ;++i){
        file.seekg(BASESIZE + i * INDEXSIZE);
        file >> keyIter;
        if(keyIter == key){
            file >> indexIter;
            beginPos = indexIter;
            if(i == sizeOfIndex - 1){
                len = -1;
            }
            else{
                file.seekg(BASESIZE + i * INDEXSIZE + 8);
                uint32_t nextindexIter = 0;
                file >> nextindexIter;
                len = nextindexIter - indexIter;
            }
            break;
        }
    }
    
    return;
}

bool DiskRun::isContain(uint64_t key){

    if(minKey > key || maxKey < key) return false;

    file.seekg(HEADERSIZE);

    filter = new bloom;

    file >> *filter;

    if(!filter->mayContain(key)){
        return false;
    }

    return true;
}

std::string DiskRun::get(uint64_t key){
    std::string value = "";

    if(isContain(key)) return value;

    uint32_t beginPos = 0;
    uint32_t len = 0;

    locateValuePos(key,beginPos,len);

    if(beginPos != 0){
        if(len == -1){
            file.seekg(beginPos);
            file >> value;
        }
        else{
            char s[len + 1];
            file.read(s,len+1);
            value = (std::string) s;
        }
    }

    return value;
}

void DiskRun::del(uint64_t key){
    if(!isContain(key)) return;

    uint32_t beginPos = 0;
    uint32_t len = 0;

    locateValuePos(key,beginPos,len);
}

void DiskRun::reset(){
    file.clear();
}