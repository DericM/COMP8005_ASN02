#include "responseschart.h"

ResponsesChart::ResponsesChart(QWidget *parent) :
    ChartView(Qt::black, parent)
{
    chart()->setTitle("Responses per Second");

}
