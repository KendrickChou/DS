#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ProgramCode = new program;
    stat = new statment;
    connect(stat,stat->printNum,this,printResult);
    connect(stat,stat->inputVar,this,input);
    connect(stat,stat->GOTO_stat,this,GOTO_Line);
    connect(stat,stat->end,this,getEND);

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
        try{
            ProgramCode->readFile(CodeFile);
        }catch (const char *errMsg){
            QMessageBox::critical(this,tr("Wrong!"),
                                  tr(errMsg),QMessageBox::Close);
            return;
        }
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
    ProgramCode->initCurLine();
    while(true){
        std::string curLine = ProgramCode->getCurLine();
        std::string curLineNum = ProgramCode->getLineNum();
        if(curLine == "REACH END") break;
        try{
            stat->initStat(curLine);
            stat->executeStat(context);
            stat->clearStat();
            if(END == 1){
                END = 0;
                break;
            }
        }catch (const char* errMsg){
            std::string err = std::string(errMsg) + " in " + curLineNum;
            QMessageBox::critical(this,tr("Wrong!"),
                                  tr(err.data()),QMessageBox::Close);
            break;
        }
    }
    printLAST();
    return;
}

void MainWindow::getInput()
{
    std::string inputLine = ui->Input_Line->text().toStdString();
    if(inputLine == "") return;
    if(checkAndexecuteCmd(inputLine)){
        ui->Input_Line->clear();
        return;
    }
    else if (checkGlobalStatement(inputLine)){
        ui->Input_Line->clear();
        return;
    }
    else{
        try{
            ProgramCode->readLine(inputLine);
        }catch (const char* errMsg){
            QMessageBox::critical(this,tr("Wrong!"),
                                  tr(errMsg),QMessageBox::Close);
        }
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
    bool flag = true;
    bool ok = false;
    QString text = "";
    int inputNum;
    do{
        text = QInputDialog::getText(this,tr("Input Identifier"),
                                             "Please input your Identifier value"
                                             ,QLineEdit::Normal,"?",&ok);
        text.trimmed();
        inputNum = text.QString::toInt(&flag);
        if(!flag) {
           int ret = QMessageBox::critical(this,tr("Wrong!"),
                                  tr("Please Input an integer!"),
                                  QMessageBox::Ok | QMessageBox::Abort,
                                  QMessageBox::Ok);
           if(ret == QMessageBox::Abort){
               throw "Wrong Input!";
           }
        }
    }while(!flag);

    stat->setIdentifier(context,inputNum);
}

void MainWindow::GOTO_Line(int lineNum){
    ProgramCode->jumpLine(lineNum);
}

void MainWindow::getEND()
{
    END = 1;
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
        std::string LAST_Line = ProgramCode->getLineNum();
        if(curLine == "REACH END") break;
        LAST_Line += " ";
        try{
            bool flag = printLASTStat->initStat(curLine);
            if(!flag){
                LAST_Line += "Error\n";
            }
            else
                LAST_Line += printLASTStat->printLAST();
        }catch (const char* erMsg){
             LAST_Line += "Error\n";
        }
        ui->LAST_Browser->append(QString::fromStdString(LAST_Line));
        delete printLASTStat;
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
              "",tr("Text files (*.txt)")));
        ProgramCode->clearLine();
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
        QDesktopServices::openUrl(QUrl::fromLocalFile("../Basic-interpreter/HELP.pdf"));
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

bool MainWindow::checkGlobalStatement(std::string statline)
{
    const char *statLine = trim(statline);
    if(!strncmp(statLine,"INPUT",5) || !strncmp(statLine,"PRINT",5)
            || !strncmp(statLine,"LET",3)){
        try{
            stat->clearStat();
            stat->initStat(statLine);
            stat->executeStat(context);
        }catch (const char* errMsg){
            std::string err = std::string(errMsg) + " in " + statLine;
            QMessageBox::critical(this,tr("Wrong!"),
                                  tr(err.data()),QMessageBox::Close);
            return true;
        }
        return true;
    }
    return false;
}
