#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cmainwindow.h"
#include "tsmainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_Client_clicked()
{
    wdg = new CMainWindow(this);
    wdg->show();
    hide();
}

void MainWindow::on_startThreadedServer_clicked()
{
    wdg = new TSMainWindow(this);
    wdg->show();
    hide();
}
