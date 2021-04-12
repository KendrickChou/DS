#ifndef EXP_H
#define EXP_H

#include "global.h"
#include <string>
#include <map>
#include <cmath>
#include <iostream>

class EvaluationContext;

enum ExpressionType {ERR_Type ,CONSTANT , IDENTIFIER , COMPOUND};

class Expression{
public:
    Expression(){}
    virtual ~Expression(){}
    virtual int eval(EvaluationContext &context) = 0;
    virtual std::string toString() = 0;
    virtual ExpressionType type() = 0;

    virtual int getConstantValue();
    virtual std::string getIdentifierName();
    virtual OPERATION getOperator();
    virtual Expression *getLHS();
    virtual Expression *getRHS();
};

class ConstantExp:public Expression{
public:
    ConstantExp(){}
    ~ConstantExp();

    ConstantExp(int val);

    virtual int eval(EvaluationContext &context);
    virtual std::string toString();
    virtual ExpressionType type();
    virtual int getConstantValue();

private:
    int value;
};
// why long?

class IdentifierExp:public Expression{
public:
    IdentifierExp(){}
    ~IdentifierExp();
    IdentifierExp(std::string name);

    virtual int eval(EvaluationContext &context);
    virtual std::string toString();
    virtual ExpressionType type();
    virtual std::string getIdentifierName();

private:
    std::string name;
};

class CompoundExp: public Expression{
public:
    CompoundExp(OPERATION op, Expression *rhs, Expression *lhs);
    ~CompoundExp();

    virtual int eval(EvaluationContext &context);
    virtual std::string toString();
    virtual ExpressionType type();

    virtual OPERATION getOperator();
    virtual Expression *getLHS();
    virtual Expression *getRHS();

private:
    OPERATION op;
    Expression *lhs,*rhs;
};

class EvaluationContext{
public:
    void setValue(std::string var, int value);
    int getValue(std::string var);
    bool isDefined(std::string var);
    void clearContext();
    void copySymbolTable(std::map<std::string,int> &dest);
    void mergeContext(EvaluationContext mergedcontext);

private:
    std::map<std::string, int> symbolTable;
};

#endif // EXP_H
