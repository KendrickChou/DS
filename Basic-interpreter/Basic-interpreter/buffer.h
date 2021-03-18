#ifndef BUFFER_H
#define BUFFER_H

#include <string>
#include <map>


class Buffer
{
private:

public:
    Buffer();
    std::map<int,std::string> *lineMap;
    void appendLine(const std::string &text);
    void deleteLine(std::string Id);
    std::string outputLine();
    void clearLine();
    void SKIP_BLANK(std::string &text);
};

#endif // BUFFER_H
