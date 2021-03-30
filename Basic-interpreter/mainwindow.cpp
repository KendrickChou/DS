#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ProgramCode = new program;

    connect(ui->Input_Line,ui->Input_Line->returnPressed,
            this,getInput);
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
        context.clearContext();
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

void MainWindow::getInput()
{
    std::string inputLine = ui->Input_Line->text().toStdString();
    if(checkAndexecuteCmd(inputLine)){
        ui->Input_Line->clear();
        return;
    }
    else{
        ProgramCode->readLine(inputLine);
        refreshCodeBrowser();
        ui->Input_Line->clear();
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

bool MainWindow::checkAndexecuteCmd(std::string cmd)
{
    const char *command = cmd.data();
    if(!strncmp(command,"RUN",3)){
        this->runCode();
        return true;
    }
    else if(!strncmp(command,"LOAD",4)){
        QFile CodeFile(QFileDialog::getOpenFileName(this,tr("Open Code File"),
              "C:\myCode\DS",tr("Text files (*.txt)")));
        ProgramCode->clearLine();
        context.clearContext();
        ui->Result_Browser->clear();
        ProgramCode->readFile(CodeFile);
        refreshCodeBrowser();
        return true;
    }
    else if(!strncmp(command,"CLEAR",5)){
        ProgramCode->clearLine();
        context.clearContext();
        ui->Result_Browser->clear();
        ui->LAST_Browser->clear();
        refreshCodeBrowser();
        return true;
    }
    else if(!strncmp(command,"HELP",4)){
        QDesktopServices::openUrl(QUrl::fromLocalFile("../Basic-interpreter/HELP.md"));
        return true;
    }
    else if(!strncmp(command,"QUIT",4)){
        this->close();
        return true;
    }
    return false;
}

char *MainWindow::trim(std::string s)
{
    char *res;
    res = strdup(s.c_str());
    while(*res == ' ') ++res;

    int i = strlen(res) - 1;
    while(i >= 0 && res[i] == ' ') --i;
    res[i + 1] = '\0';
    return res;
}

