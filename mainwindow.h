#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>、
#include <QString>
#include <QFileDialog>
#include <QFile>
#include "program.h"

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
    program *ProgramCode;
    void refreshCodeBrowser();
};
#endif // MAINWINDOW_H
