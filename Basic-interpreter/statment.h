#ifndef STATMENT_H
#define STATMENT_H

#include <parser.h>
#include "global.h"
#include "tokenizer.h"
#include <iostream>
#include <QObject>
#include <QQueue>

class statment : public QObject
{
    Q_OBJECT
public:
    ~statment();
    void initStat(const std::string &line);
    void executeStat(EvaluationContext &context);
    void setIdentifier(EvaluationContext &context,int num);
    std::string printLAST();

private:
    STATE stat;
    tokenizer *tokens;
    parser *parserExp;
    int inputNum;
    QQueue<std::string> REM_Queue;
    bool handleREM(const std::string &line);
    bool handleLET(EvaluationContext &context);
    bool handlePRINT(EvaluationContext &context);
    bool handleINPUT(EvaluationContext &context);
    bool handleGOTO(EvaluationContext &context);
    bool handleIF(EvaluationContext &context);
signals:
    void printNum(int);
    void inputVar();
    void GOTO_stat(int);
private slots:
};

#endif // STATMENT_H
