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
        refreshCodeBrowser();
    });
    connect(ui->Load_Btn,ui->Load_Btn->clicked,
            this,[=](){
        QFile CodeFile(QFileDialog::getOpenFileName(this,tr("Open Code File"),
              "C:\myCode\DS",tr("Text files (*.txt)")));
        ProgramCode->clearLine();
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
    EvaluationContext context;
    ProgramCode->initCurLine();
    while(true){
        std::string curLine = ProgramCode->getCurLine();
        if(curLine == "END") return;
        statment *stat = new statment;
        connect(stat,stat->printNum,this,printResult);
        stat->initial(curLine);
        stat->executeStat(context);
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

void MainWindow::refreshCodeBrowser(){
    ui->Code_Browser->clear();
    ui->Code_Browser->append(QString::fromStdString(ProgramCode->outputLine()));
    ui->Code_Browser->show();
}

