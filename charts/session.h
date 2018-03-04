#ifndef SESSION_H
#define SESSION_H

#include "charts/chartview.h"
#include <QtCore/QTimer>
#include <QObject>
#include <mutex>

class Session : public QObject
{
    Q_OBJECT
public:
    explicit Session(QWidget *parent = nullptr);
    ~Session();

    void addDataSent(int);
    void addDataRecv(int);
    void addClient();
    void removeClient();
    void addRequest();
    void addResponse();

signals:

public slots:
    void handleTimeout();

private:
    int clients;

    double total_data_sent;
    double total_data_recv;
    double timeslice_data_sent;
    double timeslice_data_recv;
    int timeslice_requests;
    int timeslice_responses;

    std::mutex sendLock;
    std::mutex recvLock;
    std::mutex reqLock;
    std::mutex respLock;
    std::mutex clientLock;

    ChartView *sendChart;
    ChartView *recvChart;
    ChartView *clientChart;
    ChartView *requestsChart;
    ChartView *responsesChart;
    QTimer m_timer;
};


#endif // SESSION_H
