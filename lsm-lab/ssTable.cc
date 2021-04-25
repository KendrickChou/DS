#include "ssTable.h"

SSTable::SSTable(){

}

SSTable::SSTable(std::string filePath,
    std::vector<PAIR> pairVec){
        this->filter = new bloom;
        this->Header.timestamp = TIMESTAMP++;
        this->Header.numOfkey = pairVec.size();
        this->filePath = filePath;

        file.open((char *)filePath.data(),std::ios::out|std::ios_base::trunc|std::ios_base::binary);
        this->file.write((char *)(&this->Header.timestamp),8);
        this->file.write((char *)(&this->Header.numOfkey),8);
        this->file.write((char *)(&this->Header.minKey),8);
        this->file.write((char *)(&this->Header.maxKey),8);

        for(int i = 0;i < Header.numOfkey; ++i){
            filter->addKey(pairVec[i].first);
        }

        char* filter_in_char = filter->bitset_to_bytes();
        file.write(filter_in_char,BLOOM_SIZE >> 3);
        delete filter_in_char;

        uint32_t offset = BASESIZE + Header.numOfkey * INDEXSIZE;

        for(int i = 0;i < Header.numOfkey;++i){
            file.write((char *)(&pairVec[i].first),8);
            file.write((char *)(&offset),4);
            index.emplace_back(pairVec[i].first,offset);
            offset += pairVec[i].second.size();
        }

        for(int i = 0;i < Header.numOfkey; ++i){
            file.write((char *)pairVec[i].second.data(),pairVec[i].second.size());
        }

        Header.minKey = pairVec[0].first;
        Header.maxKey = pairVec[Header.numOfkey - 1].first;

        file.close();

        file.open((char *)filePath.data(),std::ios_base::in|std::ios_base::binary);
}

SSTable::~SSTable(){
    file.close();
    if(!filter) delete filter;
}

/* input a key, find the corresponding value.
 * if exist, return beginPos and len
 * if the key is the last one. return beginPos and len = -1
 * if not, beginPos = 0, len = 0;
 */
void SSTable::locateValuePos(uint64_t key, uint32_t &beginPos, uint32_t &len){
    uint64_t keyIter = -1;
    uint32_t indexIter = 0;

    file.seekg(BASESIZE);
    for(int i = 0;i < Header.numOfkey ;++i){
        file.seekg(BASESIZE + i*INDEXSIZE,std::ios::beg);
        file.read((char *)(&keyIter),8);
        if(keyIter == key){
            uint32_t nextindexIter = 0;
            file.read((char *)(&indexIter),4);
            beginPos = indexIter;
            if(i == Header.numOfkey - 1){
                file.seekg(0,std::ios::end);
                nextindexIter = file.tellg();
            }
            else{
                file.seekg(BASESIZE + (i+1) * INDEXSIZE + 8);
                file.read((char*)(&nextindexIter),4);
            }
            len = nextindexIter - indexIter;
            break;
        }
    }
    
    return;
}

bool SSTable::isContain(uint64_t key){

    if(Header.minKey > key || Header.maxKey < key) return false;

    file.seekg(HEADERSIZE);

    if(!filter->mayContain(key)){
        return false;
    }

    return true;
}

std::string SSTable::get(uint64_t key){
    std::string value = "";

    if(!isContain(key)) return value;

    uint32_t beginPos = 0;
    uint32_t len = 0;

    locateValuePos(key,beginPos,len);

    if(beginPos != 0){
        char s[len + 1];
        s[len] = '\0';
        file.seekg(beginPos);
        if(len == -1){
            file.read(s,999999);
        }
        else{
            file.read(s,len);
        }
        value = (std::string) s;
    }

    return value;
}

void SSTable::del(uint64_t key){
}

void SSTable::reset(){
}

/*
 * getByOffset - get the value by offset
 * if value is at the end of a file, then pass end = 0;
 */

std::string SSTable::getByOffset(uint32_t begin,uint32_t end) {
    std::string value = "";

    if (end == 0) {
        file.seekg(0, std::ios::end);
        end = file.tellg();
    }

    uint32_t len = end - begin;

    file.seekg(begin);
    char *v = (char *)filePath.data();
    char s[len + 1];
    s[len] = '\0';
    file.read(s, len);
    value = (std::string) s;

    return value;
}
