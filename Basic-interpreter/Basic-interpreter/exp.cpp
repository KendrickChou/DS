#include "exp.h"
#include <iostream>

ConstantExp::ConstantExp(int val)
{
    this->value = val;
}

int ConstantExp::eval(EvaluationContext &context)
{
    return value;
}

std::string ConstantExp::toString()
{
    return std::to_string(this->value);
}

ExpressionType ConstantExp::type()
{
    return CONSTANT;
}

int ConstantExp::getConstantValue()
{
    return this->value;
}

//***************ConstantExp*********************//

IdentifierExp::IdentifierExp(std::string name)
{
    this->name = name;
}

int IdentifierExp::eval(EvaluationContext &context)
{
//    if(!context.isDefined(name)){
//        std::cout << name << "is NOT defined!" << endl;
//        return;
//    }
    if(context.isDefined(name)){
        return context.getValue(name);
    }
    return -1;
}

std::string IdentifierExp::toString()
{
    return this->getIdentifierName();
}

ExpressionType IdentifierExp::type()
{
    return IDENTIFIER;
}

std::string IdentifierExp::getIdentifierName()
{
    return this->name;
}

//*****************IdentifierExp********************//

CompoundExp::CompoundExp(OPERATION op, Expression *rhs, Expression *lhs)
{
    this->op = op;
    this->lhs = lhs;
    this->rhs = rhs;
}

int CompoundExp::eval(EvaluationContext &context)
{
    int right = this->getRHS()->eval(context);
    switch (this->op) {
    case EQ:
    {
        std::string IdentifierName = this->getLHS()->getIdentifierName();
        context.setValue(IdentifierName,right);
        return right;
    }
    case ADD:
    {
        int left = this->getLHS()->eval(context);
        return right + left;
    }
    case SUB:
    {
        int left = this->getLHS()->eval(context);
        return left - right;
    }
    case MUL:
    {
        int left = this->getLHS()->eval(context);
        return left * right;
    }
    case DIV:
    {
        int left = this->getLHS()->eval(context);
        try{
            if(right == 0){
                throw "Division by ZERO!";
            }
        }
        catch(const char *msg){
            std::cerr << msg << std::endl;
        }
        return left / right;
    }
    case EXP:
    {
        int left = this->getLHS()->eval(context);
        return pow(left,right);
    }
    }
}

std::string CompoundExp::toString()
{
    std::string compoundString;
    Expression *left = this->getLHS();
    Expression *right = this->getRHS();
    std::string opToString;
    switch (this->op) {
    case ADD:
        opToString = "+";
        break;
    case SUB:
        opToString = "-";
        break;
    case MUL:
        opToString = "*";
        break;
    case DIV:
        opToString = "/";
        break;
    case EXP:
        opToString = "**";
        break;
    default:
        break;
    }
    compoundString += opToString;
    compoundString += "\n\t";
    compoundString += left->toString();
    compoundString += "\n\t";
    compoundString += right->toString();
    delete left,right;
    return compoundString;
}

ExpressionType CompoundExp::type()
{
    return COMPOUND;
}

OPERATION CompoundExp::getOperator()
{
    return this->op;
}

Expression *CompoundExp::getLHS()
{
    return lhs;
}

Expression *CompoundExp::getRHS()
{
    return rhs;
}

//*******************CompoundExp**********************//

int Expression::getConstantValue()
{
    return -99999;
}

std::string Expression::getIdentifierName()
{
    return "error";
}

OPERATION Expression::getOperator()
{
    return ADD;
}

Expression *Expression::getLHS()
{
    return NULL;
}

Expression *Expression::getRHS()
{
    return NULL;
}

//***************Expression***********************//

void EvaluationContext::setValue(std::string var, int value)
{
    if(!isDefined(var)){
        this->symbolTable.insert(std::pair<std::string,int>(var,value));
    }
    else{
        this->symbolTable[var] = value;
    }
    return;
}

int EvaluationContext::getValue(std::string var)
{
    return this->symbolTable.find(var)->second;
}

bool EvaluationContext::isDefined(std::string var)
{
    return this->symbolTable.count(var);
}

void EvaluationContext::clearContext()
{
    symbolTable.clear();
}

void EvaluationContext::copySymbolTable(std::map<std::string, int> &dest)
{
    dest = symbolTable;
}



//****************EvaluationContext******************//
