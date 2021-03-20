#ifndef PARSER_H
#define PARSER_H

#include "tokenizer.h"
#include "exp.h"
#include <QStack>

class parser
{
private:
    QVector<token> parseTokens;
    QStack<OPERATION> operators;
    QStack<Expression *> operands;
    Expression *root;
    void handleOperator(OPERATION op);
    void handleOperand(token operand);
    void deleteTree(Expression *root);
    void printParserTree(Expression *root,std::string &print,int level);
    void addTab(std::string &print,int level);
    std::string opToString(OPERATION op);

public:
    parser();
    void setParser(QVector<token> parseTokens);
    void clearParser();
    ~parser();
    void parseExp();
    int evalExp(EvaluationContext &context);
    std::string getIdentifierName();
    std::string printLAST();
};

#endif // PARSER_H
