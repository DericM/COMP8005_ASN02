#include "clientchart.h"

ClientChart::ClientChart(QWidget *parent) :
    ChartView(Qt::green, parent)
{
    chart()->setTitle("Number of Clients");

}
