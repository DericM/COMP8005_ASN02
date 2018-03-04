#include "requestschart.h"

RequestsChart::RequestsChart(QWidget *parent) :
    ChartView(Qt::yellow, parent)
{
    chart()->setTitle("Requests per Second");

}
