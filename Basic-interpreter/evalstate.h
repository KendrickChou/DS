#ifndef EVALSTATE_H
#define EVALSTATE_H

#include <map>

class EvalState{
public:
//    void setValue(std::string var, int value);
//    int getValue(std::string var);
//    bool isDefined(std::string var);
    void setSymbolTable(std::map<std::string,int> value);
    void setCurLine(const std::string lineNum);
    std::map<std::string, int> symbolTable;
    std::string curLineNum;
};

#endif // EVALSTATE_H
