#ifndef LEVELONECHART_H
#define LEVELONECHART_H

#include <qwt_plot.h>
#include <qwt_plot_picker.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>

class LevelOneChart : public QwtPlot
{
    Q_OBJECT

    QwtPlotPicker *plotPicker;
    QwtLegend *legend;
    QwtPlotGrid *gridToPlot;

    /* Markers */
    QwtPlotMarker *mY;
    QwtPlotMarker *mX;

    /* Cureves */
    QwtPlotCurve *curveR;
    QwtPlotCurve *curveF;

    /* Symbols */
    QwtSymbol *symbolR;
    QwtSymbol *symbolF;

    void updateWidgetGradient();
    void plotAssay();
protected:
    virtual void resizeEvent(QResizeEvent *);
private slots:
    void showItem(QwtPlotItem *item, bool on);
public slots:
    void exportLevelOnePlotToImage();
    void printLevelOnePlot();
public:
    LevelOneChart(QWidget *parent = 0);

    void readDataOfVectors(const QVector<double> &vectorMu,
                   const QVector<double> &vectorMuForecast,
                   const QVector<QString> &vectorAlpha,
                   const QVector<QString> &vectorAlphaForecast);

    ~LevelOneChart();
};

#endif // LEVELONECHART_H
