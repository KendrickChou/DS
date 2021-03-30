  #include "evalstate.h"


//void EvalState::setValue(std::string var, int value)
//{
//    this->symbolTable[var] = value;
//    return;
//}

//int EvalState::getValue(std::string var)
//{
//    return this->symbolTable.find(var);
//}

//bool EvalState::isDefined(std::string var)
//{
//    return this->symbolTable.count(var);
//}

//std::map<std::string, int> EvalState::getSymbolTable() const
//{
//    return symbolTable;
//}

void EvalState::setSymbolTable(std::map<std::string, int> value)
{
    symbolTable = value;
}

void EvalState::setCurLine(const std::string lineNum)
{
    curLineNum = lineNum;
}
