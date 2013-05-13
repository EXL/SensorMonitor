#ifndef LINECHART_H
#define LINECHART_H

#include <QVector>

#include <qwt_plot.h>
#include <qwt_plot_picker.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>

class LineChart : public QwtPlot
{
    Q_OBJECT

    QVector<QString> vectorDateToChart; // Do not use
    QVector<QVector<double> > vectorSensorReadings2DToChart;

    size_t row;
    size_t col;

    QwtPlotPicker *plotPicker;
    QwtLegend *legend;
    QwtPlotGrid *gridToPlot;

    /* Markers */
    QwtPlotMarker *mY;
    QwtPlotMarker *mX;

    void updateWidgetGradient();
    void plotAssay();
protected:
    virtual void resizeEvent(QResizeEvent *);
private slots:
    void showItem(QwtPlotItem *, bool on);
public slots:
    void exportPlotToImage();
    void printPlot();
public:
    LineChart(const QVector<QString> &vectorDate,
              const QVector<QVector<double> > &vectorSensorReadings2D,
              QWidget *parent = 0);

    ~LineChart();
};

#endif // LINECHART_H
