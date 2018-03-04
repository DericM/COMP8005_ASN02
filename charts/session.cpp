#include "session.h"
#include "recvchart.h"
#include "sendchart.h"
#include "clientchart.h"
#include "requestschart.h"
#include "responseschart.h"
#include <mutex>

Session::Session(QWidget *parent) :
    QObject(parent),
    clients(0),
    total_data_sent(0),
    total_data_recv(0),
    timeslice_data_sent(0),
    timeslice_data_recv(0),
    timeslice_requests(0),
    timeslice_responses(0)
{
    sendChart = new SendChart(parent);
    sendChart->show();

    recvChart = new RecvChart(parent);
    recvChart->show();

    requestsChart = new RequestsChart(parent);
    requestsChart->show();

    responsesChart = new ResponsesChart(parent);
    responsesChart->show();

    clientChart = new ClientChart(parent);
    clientChart->show();

    QObject::connect(&m_timer, &QTimer::timeout, this, &Session::handleTimeout);
    m_timer.setInterval(1000);
    m_timer.start();
}

Session::~Session(){
    delete sendChart;
    delete recvChart;
    delete clientChart;
}

void Session::addDataSent(int data){
    sendLock.lock();
    total_data_sent += (data / 1000.0);
    timeslice_data_sent += (data / 1000.0);
    sendLock.unlock();
}

void Session::addDataRecv(int data){
    recvLock.lock();
    total_data_recv += (data / 1000.0);
    timeslice_data_recv += (data / 1000.0);
    recvLock.unlock();
}

void Session::addRequest(){
    reqLock.lock();
    timeslice_requests++;
    reqLock.unlock();
}

void Session::addResponse(){
    respLock.lock();
    timeslice_responses++;
    respLock.unlock();
}

void Session::addClient(){
    clientLock.lock();
    clients++;
    clientLock.unlock();
}

void Session::removeClient(){
    clientLock.lock();
    clients--;
    clientLock.unlock();
}

void Session::handleTimeout(){
    sendChart->addDataPoint(timeslice_data_sent);
    recvChart->addDataPoint(timeslice_data_recv);
    responsesChart->addDataPoint(timeslice_responses);
    requestsChart->addDataPoint(timeslice_requests);
    clientChart->addDataPoint(clients);
    sendLock.lock();
    timeslice_data_sent = 0;
    sendLock.unlock();
    recvLock.lock();
    timeslice_data_recv = 0;
    recvLock.unlock();
    reqLock.lock();
    timeslice_requests = 0;
    reqLock.unlock();
    respLock.lock();
    timeslice_responses = 0;
    respLock.unlock();
}
