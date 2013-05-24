#ifndef LEVELONEMUCHART_H
#define LEVELONEMUCHART_H

#include <qwt_plot.h>
#include <qwt_plot_picker.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>

class LevelOneMuChart : public QwtPlot
{
    Q_OBJECT

    QwtPlotPicker *plotPicker;
    QwtLegend *legend;
    QwtPlotGrid *gridToPlot;

    /* Markers */
    QwtPlotMarker *mY;
    QwtPlotMarker *mX;

    /* Curves */
    QwtPlotCurve *curveMuLower;
    QwtPlotCurve *curveMu;
    QwtPlotCurve *curveMuUpper;

    /* Symbols */
    QwtSymbol *symbolMuLower;
    QwtSymbol *symbolMu;
    QwtSymbol *symbolMuUpper;

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
    LevelOneMuChart(QWidget *parent = 0);

    void readDataOfVectors(const QVector<double> &vectorMuLower,
                           const QVector<double> &vectorMu,
                           const QVector<double> &vectorMuUpper,
                           const QVector<QString> &vectorAlpha);
    ~LevelOneMuChart();
};

#endif // LEVELONEMUCHART_H
