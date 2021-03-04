#include "program.h"

program::program()
{
    buffer = new Buffer;
}

program::~program(){
    delete buffer;
}

LINE_PROPERTY program::checkLine(const std::string &text){
    string Id;
    int i = 0;
    while(text[i] != ' ' && i < text.size()){
        Id += text[i];
        ++i;
    }
    for(int j = 0;j < Id.size();++j){
        if(Id[j] < '0' || Id[j] > '9') return INVALID;
    }
    if(i == text.size()) return DELETE;
    return APPEND;
}

void program::readLine(const std::string &text){
    LINE_PROPERTY TextProperty = checkLine(text);
    switch (TextProperty) {
    case APPEND:
        appendLine(text);
        break;
    case DELETE:
        deleteLine(text);
        break;
    case INVALID:
        break;
    }
}

void program::appendLine(const std::string &text){
    buffer->appendLine(text);
}

void program::deleteLine(std::string Id){
    buffer->deleteLine(Id);
}

std::string program::outputLine(){
    return buffer->outputLine();
}

void program::clearLine()
{
    buffer->clearLine();
}

void program::readFile(QFile &CodeFile)
{
    std::string FileName = CodeFile.fileName().toStdString();
    std::ifstream  file;
    file.open(FileName);
    string line;
    while(getline(file,line)){
        buffer->appendLine(line);
    }
    file.close();
}


