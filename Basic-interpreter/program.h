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

public:
    program();
    ~program();

    void readLine(const std::string &text);
    void appendLine(const std::string &text);
    void deleteLine(std::string Id);
    LINE_PROPERTY checkLine(const std::string &text);
    std::string outputLine();
    void clearLine();

    void readFile(QFile &CodeFile);
};

#endif // PROGRAM_H
