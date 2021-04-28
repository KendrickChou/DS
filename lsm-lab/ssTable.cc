#include "ssTable.h"

SSTable::SSTable(){

}

SSTable::SSTable(std::string filePath,
    uint64_t timestamp,
    const std::vector<PAIR> &pairVec){
        this->Header.timestamp = timestamp;
        this->Header.numOfkey = pairVec.size();
        this->Header.minKey = pairVec[0].first;
        this->Header.maxKey = pairVec[this->Header.numOfkey - 1].first;
        this->filePath = filePath;

        file.open(filePath,std::ios::out|std::ios_base::trunc|std::ios_base::binary);
        this->file.write((char *)(&this->Header.timestamp),8);
        this->file.write((char *)(&this->Header.numOfkey),8);
        this->file.write((char *)(&this->Header.minKey),8);
        this->file.write((char *)(&this->Header.maxKey),8);

        for(int i = 0;i < Header.numOfkey; ++i){
            filter.addKey(pairVec[i].first);
        }

        char* filter_in_char = filter.bitset_to_bytes();
        file.write(filter_in_char,BLOOM_SIZE >> 3);
        delete [] filter_in_char;

        uint32_t offset = BASESIZE + Header.numOfkey * INDEXSIZE;

        for(int i = 0;i < Header.numOfkey;++i){
            index.emplace_back(pairVec[i].first,offset);
            file.write((char *)(&pairVec[i].first),8);
            file.write((char *)(&offset),4);
            offset += pairVec[i].second.size();
        }

        for(int i = 0;i < Header.numOfkey; ++i){
            file.write((char *)pairVec[i].second.c_str(),pairVec[i].second.size());
        }

        Header.minKey = pairVec[0].first;
        Header.maxKey = pairVec[Header.numOfkey - 1].first;

    file.close();
    file.clear();
}

SSTable::~SSTable(){
}

/* input a key, find the corresponding value.
 * if exist, return beginPos and len
 * if not, beginPos = 0, len = 0;
 */
void SSTable::locateValuePos(uint64_t key, uint32_t &beginPos, uint32_t &len){
    uint64_t keyIter = -1;
    len = 0;
    beginPos = 0;
    uint64_t low = 0, high = Header.numOfkey, mid = 0;

    while(low < high){
        mid = (low + high) / 2;
        keyIter = this->index[mid].first;

        if(keyIter == key){
            uint32_t endPos = 0;
            beginPos = this->index[mid].second;
            if (mid == (Header.numOfkey - 1)) {
                file.seekg(0, std::ios::end);
                endPos = file.tellg();
                len = endPos - beginPos;
            } else {
                len = this->index[mid + 1].second - beginPos;
            }
            return;
        } else if(key < keyIter){
            high = mid;
        } else if(key > keyIter){
            low = mid + 1;
        }
    }
}

bool SSTable::isContain(uint64_t key){
    if(Header.minKey > key || Header.maxKey < key) return false;

    if(!filter.mayContain(key)){
        return false;
    }

    return true;
}

void SSTable::get(uint64_t key,std::string &value){
    file.open(filePath,std::ios::in|std::ios::binary);

    if(!isContain(key)) {
        file.close();
        file.clear();
        return;
    }

    uint32_t beginPos = 0;
    uint32_t len = 0;

    locateValuePos(key,beginPos,len);

    if(beginPos != 0){
        char s[len + 1];
        s[len] = '\0';
        file.seekg(beginPos);
        file.read(s,len);
        value =  s;
    }

    file.clear();
    file.close();
    return;
}

/*
 * getByOffset - get the value by offset
 * if value is at the end of a file, then pass end = 0;
 */

void SSTable::getByOffset(std::string &value, uint32_t begin,uint32_t end) {

    if (end == 0) {
        file.seekg(0, std::ios::end);
        end = file.tellg();
    }

    uint32_t len = end - begin;

    file.seekg(begin);
    char s[len + 1];
    s[len] = '\0';
    file.read(s, len);
    value = s;

    return;
}
