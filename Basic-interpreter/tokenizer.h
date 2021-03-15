#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>

enum OPERATION{ADD,SUB,MUL,DIV,EXP,LP,RP,EQ};

enum STATMENT{REM,LET,PRINT,INPUT,GOTO,IF,THEN,END};

enum COMMAND{RUN,LOAD,LIST,CLEAR,HELP,QUIT};

/*
 * ln: line number,
 * op: operation
 * var: variable
 * num: number
 * stat: statment
 * com: command
*/
enum TYPE{ln,op,var,num,stat,com};

class node
{
private:
    TYPE type;
    union Data{
        int LN = 0;
        OPERATION Op;
        std::string Var;
        int Num;
        std::string Stat;
        std::string Comm;
    };
    Data val;
public:
    void setNode(std::string &s);
    node *pre,*succ;
};

class tokenizer
{
public:


   tokenizer();
   node *getHead();
   int getLength();
   void transStringtoList(const std::string &text);
   node *getNodeI(int I);
private:
   node *head;
   int length;
};

#endif // TOKENIZER_H
