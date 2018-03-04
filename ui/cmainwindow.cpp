#include "cmainwindow.h"
#include "ui_cmainwindow.h"

#include <QtCharts/QChartView>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>

CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CMainWindow)
{
    ui->setupUi(this);
}

CMainWindow::~CMainWindow()
{
    client->stop();
    delete ui;
    //delete client;
    delete session;
}

void CMainWindow::on_start_clicked()
{
    std::string host = ui->ip->text().toStdString();
    int port = ui->port->text().toInt();
    int clients = ui->clients->text().toInt();
    int packetSize = ui->packetSize->text().toInt();

    session = new Session(this);

    client = new Client(host, port, session);

    client->run(clients, packetSize);

    ui->start->setEnabled(false);
    ui->port->setEnabled(false);
    ui->ip->setEnabled(false);
    ui->clients->setEnabled(false);
    ui->packetSize->setEnabled(false);
}
