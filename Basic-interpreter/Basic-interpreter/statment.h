#ifndef STATMENT_H
#define STATMENT_H

#include <parser.h>
#include "global.h"
#include "tokenizer.h"
#include <iostream>
#include <QObject>

class statment : public QObject
{
    Q_OBJECT
public:
    ~statment();
    void initial(const std::string &line);
    void executeStat(EvaluationContext &context);

private:
    STATE stat;
    tokenizer *tokens;
    bool handleREM(EvaluationContext &context);
    bool handleLET(EvaluationContext &context);
    bool handlePRINT(EvaluationContext &context);
    bool handleINPUT(EvaluationContext &context);
    bool handleGOTO(EvaluationContext &context);
    bool handleIF(EvaluationContext &context);
    bool handleEND(EvaluationContext &context);
signals:
    void printNum(int);
};

#endif // STATMENT_H
