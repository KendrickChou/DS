#include "parser.h"

parser::parser()
{
    root = NULL;
}

void parser::setParser(QVector<token> parseTokens)
{
    this->parseTokens.clear();
    this->parseTokens = parseTokens;

    this->parseExp();
}

void parser::clearParser()
{
}

parser::~parser()
{
    deleteTree(root);
}

void parser::deleteTree(Expression *root)
{
    if(root == NULL) return;
    switch (root->type()){
    case CONSTANT:
    case IDENTIFIER:
        delete root;
        break;
    case COMPOUND:{
        deleteTree(root->getLHS());
        deleteTree(root->getRHS());
        delete root;
        break;
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

std::string parser::getIdentifierName()
{
    return root->getIdentifierName();
}

std::string parser::printLAST()
{
    std::string print;
    printParserTree(this->root,print,1);
    return print;
}

void parser::addTab(std::string &print, int level)
{
    for(int i = 0;i < level;++i){
        print += "    ";
    }
}

std::string parser::opToString(OPERATION op)
{
    switch (op) {
    case ADD:
        return "+";
    case SUB:
        return "-";
    case MUL:
        return "*";
    case DIV:
        return "/";
    case EXP:
        return "**";
    case LP:
        return "(";
    case RP:
        return ")";
    case EQ:
        return "=";
    case GT:
        return ">";
    case LT:
        return "<";
    }
}

void parser::printParserTree(Expression *root,std::string &print, int level)
{
    switch (root->type()) {
    case CONSTANT:
    {
        addTab(print,level);
        print += std::to_string(root->getConstantValue());
        print += '\n';
        break;
    }
    case IDENTIFIER:
    {
        addTab(print,level);
        print += root->getIdentifierName();
        print += '\n';
        break;
    }
    case COMPOUND:
    {
       addTab(print,level);
       print += opToString(root->getOperator());
       print += '\n';
       printParserTree(root->getLHS(),print,level + 1);
       printParserTree(root->getRHS(),print,level + 1);
       break;
    }
    }
    return;
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
        while(!operators.empty() && operators.top() != LP && operators.top() != EQ){
            CompoundExp *compound = new CompoundExp(operators.pop(),operands.pop(),operands.pop());
            operands.append(compound);
        }
        operators.append(op);
        break;
    }
    case MUL:
    case DIV:
    {
        while(!operators.empty() && (operators.top() == MUL || operators.top() == DIV || operators.top() == EXP)){
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
        operators.append(op);
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