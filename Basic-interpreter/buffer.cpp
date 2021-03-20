#include "buffer.h"

Buffer::Buffer()
{
    lineMap = new std::map<int,std::string>;
}



void Buffer::appendLine(const std::string &text)
{
    char *ptr;
    char *cText = (char *)text.data();
    int lineNum = strtol(cText,&ptr,10);
    if(lineMap->count((lineNum))){
        lineMap->erase(lineNum);
    }
    lineMap->insert(std::pair<int,std::string>(lineNum,std::string(ptr)));
}

void Buffer::deleteLine(std::string Id)
{
    int lineNum = std::atoi(Id.c_str());
    lineMap->erase(lineNum);
}

std::string Buffer::outputLine()
{
    std::string outputString;
    std::map<int,std::string>::iterator iter;
    for(iter = lineMap->begin(); iter != lineMap->end(); ++iter){
        outputString += (std::to_string(iter->first) + iter->second);
        outputString += '\n';
    }
    return outputString;
}

void Buffer::clearLine()
{
    lineMap->clear();
}

void Buffer::SKIP_BLANK(std::string &text)
{
    char *cText = (char *)text.data();
    while(*cText == ' '){
        ++cText;
    }
    text = cText;
}
