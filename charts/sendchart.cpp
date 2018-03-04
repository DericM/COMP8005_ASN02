#include "sendchart.h"

SendChart::SendChart(QWidget *parent) :
    ChartView(Qt::blue, parent)
{
    chart()->setTitle("Sent Data(kb/s)");

}
