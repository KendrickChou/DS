#include "statment.h"

statment::~statment()
{
    delete parserExp;
    delete tokens;
}

void statment::initStat(const std::string &line)
{
    tokens = new tokenizer;
    parserExp = new parser;
    tokens->tokenizeLine(line);
    this->stat = (tokens->tokenVec->begin())->stat;
    tokens->tokenVec->pop_front();
    if(stat == REM){
        handleREM(line);
    }
    else if(stat == ERR){
        return;
    }
    else if(stat != IF && stat != END){
        parserExp->setParser(*(tokens->tokenVec));
    }
}

void statment::executeStat(EvaluationContext &context)
{
    switch (stat) {
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
        break;
    default:
        throw "Invalid Expression";
        break;
    }
}

void statment::setIdentifier(EvaluationContext &context, int num)
{
    std::string Id = parserExp->getIdentifierName();
    context.setValue(Id,num);
}

std::string statment::printLAST()
{
    std::string LAST_String;
    switch (this->stat) {
    case REM:
    {
        LAST_String += "REM\n    ";
        LAST_String += REM_Queue.first();
        REM_Queue.pop_front();
        LAST_String += "\n";
        break;
    }
    case INPUT:
    {
        LAST_String += "INPUT\n    ";
        LAST_String += tokens->tokenVec->begin()->var;
        LAST_String += "\n";
        break;
    }
    case GOTO:
    {
        LAST_String += "GOTO\n    ";
        LAST_String += std::to_string(tokens->tokenVec->begin()->num);
        LAST_String += "\n";
        break;
    }
    case LET:
    {
        LAST_String += "LET";
        LAST_String += parserExp->printLAST();
        break;
    }
    case PRINT:
    {
        LAST_String += "PRINT";
        LAST_String += "\n";
        LAST_String += parserExp->printLAST();
        break;
    }
    case IF:
    {
        parser Exp1,Exp2;
        QVector<token> token1,token2,thenToken;
        QVector<token>::iterator iter = tokens->tokenVec->begin();
        OPERATION op;
        bool doit = false;
        if(iter != tokens->tokenVec->end()){
            while(iter != tokens->tokenVec->end() && iter->op != GT && iter->op != LT && iter->op != EQ){
                token1.append(*iter);
                ++iter;
            }
            op = iter->op;
            ++iter;
           while(iter != tokens->tokenVec->end() && iter->stat != THEN){
               token2.append(*iter);
               ++iter;
           }
           ++iter;
           while(iter != tokens->tokenVec->end()){
               thenToken.append(*iter);
               ++iter;
           }
        }
        Exp1.setParser(token1);
        Exp2.setParser(token2);
        LAST_String += "IF  THEN\n";
        LAST_String += Exp1.printLAST();
        LAST_String += "    ";
        switch (op) {
        case EQ:
            LAST_String += "=\n";
            break;
        case GT:
            LAST_String += ">\n";
            break;
        case LT:
            LAST_String += "<\n";
            break;
        }
        LAST_String += Exp2.printLAST();
        LAST_String += "    ";
        LAST_String += std::to_string(thenToken.begin()->num);
        LAST_String += "\n";
        break;
    }
    case END:
    {
        LAST_String += "END\n";
        break;
    }
    default:
        break;
    }
    return LAST_String;
}

bool statment::handleREM(const std::string &line)
{
    const char *REM_String = line.c_str();
    while(*REM_String == ' ') ++REM_String;
    REM_String += 4; //Skip REM and ‘ ’
    REM_Queue.append(REM_String);
    return true;
}

bool statment::handleLET(EvaluationContext &context)
{
    parserExp->evalExp(context);
    return true;
}

bool statment::handlePRINT(EvaluationContext &context)
{
    int pNum = parserExp->evalExp(context);
    emit printNum(pNum);
    return true;
}

bool statment::handleINPUT(EvaluationContext &context)
{
    emit inputVar();

}

bool statment::handleGOTO(EvaluationContext &context)
{
    emit GOTO_stat(parserExp->evalExp(context));
}

bool statment::handleIF(EvaluationContext &context)
{
    parser Exp1,Exp2;
    QVector<token> token1,token2,thenToken;
    QVector<token>::iterator iter = tokens->tokenVec->begin();
    OPERATION op;
    bool doit = false;
    if(iter != tokens->tokenVec->end()){
        while(iter != tokens->tokenVec->end() && iter->op != GT && iter->op != LT && iter->op != EQ){
            token1.append(*iter);
            ++iter;
        }
        if(iter == tokens->tokenVec->end()){
            throw "Missing Compare Operation!";
        }
        op = iter->op;
        ++iter;
       while(iter != tokens->tokenVec->end() && iter->stat != THEN){
           token2.append(*iter);
           ++iter;
       }
       if(iter == tokens->tokenVec->end()){
           throw "Missing THEN!";
       }
       ++iter;
       while(iter != tokens->tokenVec->end()){
           thenToken.append(*iter);
           ++iter;
       }
    }
    else{
        return false;
    }
    Exp1.setParser(token1);
    Exp2.setParser(token2);
    switch (op) {
    case EQ:
    {
        if(Exp1.evalExp(context) == Exp2.evalExp(context)){
            doit = true;
        }
        break;
    }
    case GT:
    {
        if(Exp1.evalExp(context) > Exp2.evalExp((context))){
            doit = true;
        }
        break;
    }
    case LT:{
        if(Exp1.evalExp(context) < Exp2.evalExp(context)){
            doit = true;
        }
    }
    }
    if(doit){
        *(tokens->tokenVec) = thenToken;
        parserExp->setParser(thenToken);
        this->handleGOTO(context);
    }
    return true;
}
