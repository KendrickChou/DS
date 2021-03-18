#include "statment.h"

statment::~statment()
{
    delete tokens;
}

void statment::initial(const std::string &line)
{
    tokens = new tokenizer;

    tokens->tokenizeLine(line);
    this->stat = (tokens->tokenVec->begin())->stat;
    tokens->tokenVec->pop_front();
}

void statment::executeStat(EvaluationContext &context)
{
    switch (stat) {
    case REM:
        handleREM(context);
        break;
    case LET:
        handleLET(context);
        break;
    case PRINT:
        handlePRINT(context);
        break;
    case INPUT:
        handleINPUT(context);
        break;
    case GOTO:
        handleGOTO(context);
        break;
    case IF:
        handleIF(context);
        break;
    case END:
        handleEND(context);
        break;
    default:
        break;
    }
}

bool statment::handleREM(EvaluationContext &context)
{
    return true;
}

bool statment::handleLET(EvaluationContext &context)
{
    parser *exp = new parser(*(tokens->tokenVec));
    exp->evalExp(context);
    return true;
}

bool statment::handlePRINT(EvaluationContext &context)
{
    parser *exp = new parser(*(tokens->tokenVec));
    int pNum = exp->evalExp(context);
    emit printNum(pNum);
    return true;
}

bool statment::handleINPUT(EvaluationContext &context)
{

}

bool statment::handleGOTO(EvaluationContext &context)
{

}

bool statment::handleIF(EvaluationContext &context)
{

}

bool statment::handleEND(EvaluationContext &context)
{

}
