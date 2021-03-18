#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <QVector>
#include "global.h"

/*
 * EXP: '**';
 * LP: '('
 * RP: ')'
 * EQ: '='
*/
//enum OPERATION{ADD,SUB,MUL,DIV,EXP,LP,RP,EQ};

//enum STATE{ERR,REM,LET,PRINT,INPUT,GOTO,IF,THEN,END};

//enum TOKENIZE_TYPE{T_ERR,T_NUM,T_OP,T_VAR,T_STAT};

/*
 * OP: operation
 * VAR: variable
 * NUM: number
 * STAT: STATMENT
*/
//enum TOKEN_TYPE{OP,VAR,NUM,STAT};

enum TOKENIZE_TYPE{T_ERR,T_NUM,T_OP,T_VAR,T_STAT};

class token{
public:
    TOKEN_TYPE type;
    OPERATION op;
    STATE stat;
    long num;
    std::string var;
};

class tokenizer
{
public:
   tokenizer();
   QVector<token> *tokenVec;
   void tokenizeLine(const std::string &line);

private:
    TOKENIZE_TYPE tokenizeNum(char *&tline,long &num);
    TOKENIZE_TYPE tokenizeOp(char *&tline,OPERATION &op);
    TOKENIZE_TYPE tokenizeVarOrStat(char *&tline,std::string &var,STATE &stat);
    void SKIP_BLANK(char *&line);
    STATE checkStat(char *&stat);
};

#endif // TOKENIZER_H
