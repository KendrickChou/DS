#include "parser.h"

parser::parser()
{
}

parser::parser(QVector<token> parseTokens)
{
    this->parseTokens = parseTokens;

    this->parseExp();
}

parser::~parser()
{
    deleteTree(root);
}

void parser::deleteTree(Expression *root)
{
    switch (root->type()) {
    case CONSTANT:
    case IDENTIFIER:
        delete root;
    case COMPOUND:{
        deleteTree(root->getLHS());
        deleteTree(root->getRHS());
    }
    }
}

void parser::parseExp()
{
    for(int i = 0;i < parseTokens.size();++i){
        switch (parseTokens[i].type) {
        case STAT:
            printf("Wrong Expression!");
            break;
        case OP:
        {
            handleOperator(parseTokens[i].op);
            break;
        }
        default:
            handleOperand(parseTokens[i]);
            break;
        }
    }
    while(!operators.empty()){
        CompoundExp *compound = new CompoundExp(operators.pop(),operands.pop(),operands.pop());
        operands.append(compound);
    }
    root = operands.pop();
}

int parser::evalExp(EvaluationContext &context)
{
   return root->eval(context);
}

void parser::handleOperand(token operand)
{
    switch (operand.type) {
    case NUM:
    {
        ConstantExp *constant =  new ConstantExp(operand.num);
        operands.append(constant);
        break;
    }
    case VAR:
    {
        IdentifierExp *var = new IdentifierExp(operand.var);
        operands.append(var);
        break;
    }
    default:
        break;
    }
}

void parser::handleOperator(OPERATION op)
{
    switch (op) {
    case ADD:
    case SUB:
    {
        while(operators.top() == ADD || operators.top() == SUB){
            CompoundExp *compound = new CompoundExp(operators.pop(),operands.pop(),operands.pop());
            operands.append(compound);
        }
        operators.append(op);
        break;
    }
    case MUL:
    case DIV:
    {
        while(operators.top() == MUL || operators.top() == DIV){
            CompoundExp *compound = new CompoundExp(operators.pop(),operands.pop(),operands.pop());
            operands.append(compound);
        }
        operators.append(op);
        break;
    }
    case LP:
    {
        operators.append(op);
        break;
    }
    case RP:
    {
        while(operators.top() != LP){
            CompoundExp *compound = new CompoundExp(operators.pop(),operands.pop(),operands.pop());
            operands.append(compound);
        }
        operators.pop();
        break;
    }
    case EXP:
    {
        operators.append(EXP);
        while(operators.top() == EXP){
            CompoundExp *compound = new CompoundExp(op,operands.pop(),operands.pop());
            operands.append(compound);
        }
        break;
    }
    case EQ:
    {
        operators.append(op);
        break;
    }
    default:
        break;
    }
}
