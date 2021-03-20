#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ProgramCode = new program;

    connect(ui->Input_Line,ui->Input_Line->returnPressed,
            this,[=] (){
        ProgramCode->readLine(ui->Input_Line->text().toStdString());
        refreshCodeBrowser();
        ui->Input_Line->clear();
    });
    connect(ui->Clear_Btn,ui->Clear_Btn->clicked,
            ui->Code_Browser,[=](){
        ProgramCode->clearLine();
        context.clearContext();
        ui->Result_Browser->clear();
        ui->LAST_Browser->clear();
        refreshCodeBrowser();
    });
    connect(ui->Load_Btn,ui->Load_Btn->clicked,
            this,[=](){
        QFile CodeFile(QFileDialog::getOpenFileName(this,tr("Open Code File"),
              "C:\myCode\DS",tr("Text files (*.txt)")));
        ProgramCode->clearLine();
        context.clearContext();
        ui->Result_Browser->clear();
        ProgramCode->readFile(CodeFile);
        refreshCodeBrowser();
    });
    connect(ui->Run_Btn,ui->Run_Btn->clicked,this,runCode);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete ProgramCode;
}

void MainWindow::runCode()
{
    printLAST();
    ProgramCode->initCurLine();
    while(true){
        std::string curLine = ProgramCode->getCurLine();
        if(curLine == "REACH END") return;
        stat = new statment;
        connect(stat,stat->printNum,this,printResult);
        connect(stat,stat->inputVar,this,input);
        connect(stat,stat->GOTO_stat,this,GOTO_Line);
        stat->initial(curLine);
        stat->executeStat(context);
        evalStat.setCurLine(ProgramCode->getLineNum());
        context.copySymbolTable(evalStat.symbolTable);
        disconnect(stat,stat->GOTO_stat,this,GOTO_Line);
        disconnect(stat,stat->inputVar,this,input);
        disconnect(stat,stat->printNum,this,printResult);
        delete stat;
    }
}

void MainWindow::printResult(int pNum)
{
    QString printLine = QString::number(pNum);
    ui->Result_Browser->append(printLine);
    ui->Result_Browser->show();
}

void MainWindow::input()
{
    bool ok = false;
    QString text = QInputDialog::getText(this,tr("Input Identifier"),
                                         "Please input your Identifier value"
                                         ,QLineEdit::Normal,"?",&ok);
    int inputNum = text.QString::toInt();
    stat->setIdentifier(context,inputNum);
}

void MainWindow::GOTO_Line(int lineNum){
    ProgramCode->jumpLine(lineNum);
}

void MainWindow::refreshCodeBrowser(){
    ui->Code_Browser->clear();
    ui->Code_Browser->append(QString::fromStdString(ProgramCode->outputLine()));
    ui->Code_Browser->show();
}

void MainWindow::printLAST()
{
    ui->LAST_Browser->clear();
    ProgramCode->initCurLine();
    while(true){
        statment *printLASTStat = new statment;
        std::string curLine = ProgramCode->getCurLine();
        if(curLine == "REACH END") break;
        std::string LAST_Line = ProgramCode->getLineNum();
        LAST_Line += " ";
        printLASTStat->initial(curLine);
        LAST_Line += printLASTStat->printLAST();
        ui->LAST_Browser->append(QString::fromStdString(LAST_Line));
    }
}

