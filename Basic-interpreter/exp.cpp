#include "exp.h"


//void EvaluationContext::setValue(std::string var, int value)
//{
//    this->symbolTable[var] = value;
//    return;
//}

//int EvaluationContext::getValue(std::string var)
//{
//    return this->symbolTable.find(var);
//}

//bool EvaluationContext::isDefined(std::string var)
//{
//    return this->symbolTable.count(var);
//}

//int ConstantExp::eval(EvaluationContext &context)
//{
//    return value;
//}

//int IdentifierExp::eval(EvaluationContext &context)
//{
//    if(!context.isDefined(name)){
//        error(name + "is undefined!");
//    }
//    return context.getValue(name);
//}

//int CompoundExp::eval(EvaluationContext &context)
//{
//    int right = rhs->eval(context);
//    if(op == "="){
//        context.setValue(lhs->getIdentifierName(),right);
//        return right;
//    }

//    int left = lhs->eval(context);
//    switch (op) {
//    case "+":
//        return right + left;
//    case "-":
//        return left - right;
//    case "*":
//        return left * right;
//    case "/":
//        if(right == 0){
//            error("NAN");
//        }
//        return left / right;
//    default:
//        error("invalid expression!");
//    }
//}

//Expression *CompoundExp::getLHS()
//{
//    return lhs;
//}

//Expression *CompoundExp::getRHS()
//{
//    return rhs;
//}
