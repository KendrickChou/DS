#include "tokenizer.h"

tokenizer::tokenizer(){
    tokenVec = new QVector<token>;
}

void tokenizer::tokenizeLine(const std::string &line)
{
    char *tline = (char *)line.data();
    SKIP_BLANK(tline);
    bool ERR_FLAG = false;

    while(*tline != '\0'){
        SKIP_BLANK(tline);
        token *newToken = new token;
        if(tokenizeNum(tline,newToken->num) == T_NUM){
            newToken->type = NUM;
        }
        else if(tokenizeOp(tline,newToken->op) == T_OP){
            newToken->type = OP;
        }
        else{
            TOKENIZE_TYPE type = tokenizeVarOrStat(tline,newToken->var,newToken->stat);
            if(type == T_VAR){
                newToken->type = VAR;
            }
            else if(type == T_STAT){
                newToken->type = STAT;
            }
            else{
                ERR_FLAG = true;
                break;
            }
        }
        tokenVec->append(*newToken);
    }

    if(ERR_FLAG){
        printf("BUG IN YOUR CODE!");
        return;
    }
}

TOKENIZE_TYPE tokenizer::tokenizeNum(char *&tline,long &num)
{
    SKIP_BLANK(tline);
    char *ptr;
    num = std::strtol(tline,&ptr,0);
    if(num == 0 && ptr == tline){
        return T_ERR;
    }
    tline = ptr;
    return T_NUM;
}

TOKENIZE_TYPE tokenizer::tokenizeOp(char *&tline,OPERATION &op)
{
    SKIP_BLANK(tline);
    switch (*tline) {
    case '+':
        op = ADD;
        break;
    case '-':
        op = SUB;
        break;
    case '*':
        if(*(tline + 1) == '*'){
            op = EXP;
            ++tline;
        }
        else op = MUL;
        break;
    case '/':
        op = DIV;
        break;
    case '(':
        op = LP;
        break;
    case ')':
        op = RP;
        break;
    case '=':
        op = EQ;
        break;
    default:
        return T_ERR;
    }
    ++tline;
    return T_OP;
}

STATE tokenizer::checkStat(char *&stat)
{
    if(!strncmp(stat,"REM",3)) {
        stat += 3;
        return REM;
    }
    if(!strncmp(stat,"LET",3)) {
        stat += 3;
        return LET;
    }
    if(!strncmp(stat,"PRINT",5)){
        stat += 5;
        return PRINT;
    }
    if(!strncmp(stat,"INPUT",5)){
        stat += 5;
        return INPUT;
    }
    if(!strncmp(stat,"GOTO",4)){
        stat += 4;
        return GOTO;
    }
    if(!strncmp(stat,"IF",2)){
        stat += 2;
        return IF;
    }
    if(!strncmp(stat,"THEN",4)){
        stat += 4;
        return THEN;
    }
    if(!strncmp(stat,"END",3)){
        stat += 3;
        return END;
    }
    return ERR;
}

TOKENIZE_TYPE tokenizer::tokenizeVarOrStat(char *&tline,std::string &var,STATE &stat)
{
    SKIP_BLANK(tline);
    char *ptr = tline;
    int len = 0;
    stat = checkStat(tline);
    if(stat != ERR) return T_STAT;
    if(isalpha(*ptr)){
        while(isalpha(*ptr) || isdigit(*ptr)){
            ++len;
            ++ptr;
        }
        if(len != 0){
            char *cVar = new char[len + 1];
            strncpy(cVar,tline,len);
            *(cVar + len) = '\0';
            var = cVar;
            tline = ptr;
            return T_VAR;
        }
    }
    return T_ERR;
}

void tokenizer::SKIP_BLANK(char *&line)
{
    while((*line) == ' '){
        ++line;
    }
}
