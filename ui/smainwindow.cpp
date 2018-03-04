#include "smainwindow.h"
#include "ui_smainwindow.h"

SMainWindow::SMainWindow( ServerType type, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SMainWindow),
    type_(type)
{
    ui->setupUi(this);

    switch(type_) {
        case ServerType::Threaded :
            setWindowTitle("Threaded Server");
            break;
        case ServerType::Select :
            setWindowTitle("Select Server");
            break;
        case ServerType::Epoll :
            setWindowTitle("Epoll Server");
            break;
    }
}

SMainWindow::~SMainWindow()
{
    delete ui;
}

void SMainWindow::on_Start_clicked()
{
    int port = ui->port->text().toInt();

    session = new Session(this);

    switch(type_) {
        case ServerType::Threaded :
            server = new ThreadedServer(port, session);
            break;
        case ServerType::Select :
            server = new SelectServer(port, session);
            break;
        case ServerType::Epoll :
            server = new EpollServer(port, session);
            break;
    }

    server->run();

    ui->Start->setEnabled(false);
    ui->port->setEnabled(false);
}
