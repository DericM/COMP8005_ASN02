#ifndef TSMAINWINDOW_H
#define TSMAINWINDOW_H

#include <QMainWindow>
#include "servers/threadedserver.h"
#include "servers/selectserver.h"
#include "servers/epollserver.h"
#include "charts/session.h"

namespace Ui {
class SMainWindow;
}

enum class ServerType { Threaded, Select, Epoll};

class SMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit SMainWindow(ServerType type, QWidget *parent = 0);
    ~SMainWindow();

private slots:
    void on_Start_clicked();

private:
    Ui::SMainWindow *ui;
    Server *server;
    Session *session;
    ServerType type_;
};

#endif // TSMAINWINDOW_H
