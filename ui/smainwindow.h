#ifndef TSMAINWINDOW_H
#define TSMAINWINDOW_H

#include <QMainWindow>
#include "servers/threadedserver.h"

namespace Ui {
class TSMainWindow;
}

class TSMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TSMainWindow(QWidget *parent = 0);
    ~TSMainWindow();

private slots:
    void on_Start_clicked();

private:
    Ui::TSMainWindow *ui;
    ThreadedServer *server;
};

#endif // TSMAINWINDOW_H
