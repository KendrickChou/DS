#ifndef PROGRAM_H
#define PROGRAM_H

#include <QFile>
#include <iostream>
#include <fstream>
#include "Buffer.h"

enum LINE_PROPERTY {DELETE,APPEND,INVALID};

class program
{
private:
    Buffer *buffer;
    std::map<int,std::string>::iterator curLine;

public:
    program();
    ~program();

    void readLine(const std::string &text);
    void appendLine(const std::string &text);
    void deleteLine(std::string Id);
    LINE_PROPERTY checkLine(const std::string &text);
    std::string outputLine();
    std::string getCurLine();
    std::string getLineNum();
    void jumpLine(int Id);
    void initCurLine();
    void clearLine();

    void readFile(QFile &CodeFile);

public slots:
};

#endif // PROGRAM_H
