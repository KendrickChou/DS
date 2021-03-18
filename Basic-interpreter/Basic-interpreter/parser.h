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

public:
    parser();
    parser(QVector<token> parseTokens);
    ~parser();
    void parseExp();
    int evalExp(EvaluationContext &context);
};

#endif // PARSER_H
