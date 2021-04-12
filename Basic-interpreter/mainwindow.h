#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFileDialog>
#include <QFile>
#include <qinputdialog.h>
#include <QMessageBox>
#include <QDesktopServices>
#include "program.h"
#include "statment.h"
#include "evalstate.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    program *ProgramCode = nullptr;
    statment *stat = nullptr;
    EvaluationContext context;
    EvalState evalStat;
    bool END = 0;
    void refreshCodeBrowser();
    void printLAST();
    bool checkAndexecuteCmd(std::string cmd);
    char *trim(std::string s);
    bool checkGlobalStatement(std::string statline);
private slots:
    void runCode();
    void getInput();
    void printResult(int);
    void input();
    void GOTO_Line(int);
    void getEND();
signals:

};
#endif // MAINWINDOW_H
