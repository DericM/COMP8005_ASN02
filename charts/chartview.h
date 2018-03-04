#ifndef CHARTVIEW_H
#define CHARTVIEW_H

#include <QtCharts/QChartView>

QT_CHARTS_BEGIN_NAMESPACE
class QSplineSeries;
class QValueAxis;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class ChartView : public QChartView
{
    Q_OBJECT
public:
    explicit ChartView(Qt::GlobalColor, QWidget *parent = 0);
    void addDataPoint(int data);

private:
    QSplineSeries *series_;
    qreal maxX_;
    qreal maxY_;
};
#endif // CHARTVIEW_H
