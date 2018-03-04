#include "recvchart.h"

RecvChart::RecvChart(QWidget *parent) :
    ChartView(Qt::red, parent)
{
    chart()->setTitle("Received Data(kb/s)");

}
