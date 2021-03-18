#ifndef GLOBAL_H
#define GLOBAL_H

/*
 * EXP: '**';
 * LP: '('
 * RP: ')'
 * EQ: '='
*/
enum OPERATION{ADD,SUB,MUL,DIV,EXP,LP,RP,EQ};

enum STATE{ERR,REM,LET,PRINT,INPUT,GOTO,IF,THEN,END};

/*
 * OP: operation
 * VAR: variable
 * NUM: number
 * STAT: STATMENT
*/
enum TOKEN_TYPE{OP,VAR,NUM,STAT};


#endif // GLOBAL_H
