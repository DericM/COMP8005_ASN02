#include "cmainwindow.h"
#include "ui_cmainwindow.h"

CMainWindow::CMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CMainWindow)
{
    ui->setupUi(this);
}

CMainWindow::~CMainWindow()
{
    delete ui;
}

void CMainWindow::on_start_clicked()
{
    string host = ui->ip->text().toStdString();
    int port = ui->port->text().toInt();
    client = new Client(host, port);
    client->run();
}
