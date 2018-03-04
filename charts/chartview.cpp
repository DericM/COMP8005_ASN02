#include "chartview.h"
#include <QtCharts/QAbstractAxis>
#include <QtCharts/QSplineSeries>
#include <QtCharts/QValueAxis>
#include <QtCore/QDebug>

ChartView::ChartView(Qt::GlobalColor color, QWidget *parent) :
    QChartView(new QChart(), parent),
    maxX_(0),
    maxY_(0)
{
    setRenderHint(QPainter::Antialiasing);
    setWindowFlag(Qt::Window);
    setFixedSize(700, 450);
    chart()->legend()->hide();
    //chart()->setAnimationOptions(QChart::AllAnimations);

    series_ = new QSplineSeries(this);
    QPen pen(color);
    pen.setWidth(3);
    series_->setPen(pen);
    series_->append(maxX_, maxY_);

    chart()->addSeries(series_);
    chart()->createDefaultAxes();

    chart()->axisX()->setMin(0);
    chart()->axisY()->setMin(0);
}

void ChartView::addDataPoint(int data) {
    maxX_++;
    if(data > maxY_){
        maxY_ = data + (data * 0.02);
    }
    series_->append(maxX_, data);
    chart()->axisX()->setMax(maxX_);
    chart()->axisY()->setMax(maxY_);
}
