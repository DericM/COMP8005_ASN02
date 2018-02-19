#include "tsmainwindow.h"
#include "ui_tsmainwindow.h"
#include <QDebug>

TSMainWindow::TSMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TSMainWindow)
{
    ui->setupUi(this);
}

TSMainWindow::~TSMainWindow()
{
    delete ui;
}

void TSMainWindow::on_Start_clicked()
{
    int port = ui->port->text().toInt();
    qInfo() << "TSMainWindow::on_Start_clicked" << "port:" << port;
    server = new ThreadedServer(port);
    server->run();
}
