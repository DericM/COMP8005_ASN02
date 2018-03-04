#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cmainwindow.h"
#include "smainwindow.h"

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

void MainWindow::on_ThreadedServer_clicked()
{
    wdg = new SMainWindow(ServerType::Threaded, this);
    wdg->show();
    hide();
}

void MainWindow::on_SelectServer_clicked()
{
    wdg = new SMainWindow(ServerType::Select, this);
    wdg->show();
    hide();
}

void MainWindow::on_EpollServer_clicked()
{
    wdg = new SMainWindow(ServerType::Epoll, this);
    wdg->show();
    hide();
}

