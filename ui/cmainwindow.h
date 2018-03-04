#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QMainWindow>
#include <QtCharts/QChartView>
#include "client/client.h"
#include "charts/session.h"

namespace Ui {
class CMainWindow;
}

class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CMainWindow(QWidget *parent = 0);
    ~CMainWindow();

private slots:
    void on_start_clicked();

private:
    Ui::CMainWindow *ui;
    Client *client;
    Session *session;
};

#endif // CMAINWINDOW_H
