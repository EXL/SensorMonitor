#ifndef LEVELTWOCHART_H
#define LEVELTWOCHART_H

#include <qwt_plot.h>
#include <qwt_plot_picker.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>

class LevelTwoChart : public QwtPlot
{
    Q_OBJECT

    QwtPlotPicker *plotPicker;
    QwtLegend *legend;
    QwtPlotGrid *gridToPlot;

    /* Markers */
    QwtPlotMarker *mY;
    QwtPlotMarker *mX;

    /* Cureves */
    QwtPlotCurve *curveRA;
    QwtPlotCurve *curveFA;
    QwtPlotCurve *curveRB;
    QwtPlotCurve *curveFB;
    QwtPlotCurve *curveRC;
    QwtPlotCurve *curveFC;

    /* Symbols */
    QwtSymbol *symbolRA;
    QwtSymbol *symbolFA;
    QwtSymbol *symbolRB;
    QwtSymbol *symbolFB;
    QwtSymbol *symbolRC;
    QwtSymbol *symbolFC;

    void updateWidgetGradient();
    void plotAssay();
protected:
    virtual void resizeEvent(QResizeEvent *event);
private slots:
    void showItem(QwtPlotItem *item, bool on);
public slots:
    void exportLevelTwoPlotToImage();
    void printLevelTwoPlot();
public:
    LevelTwoChart(QWidget *parent = 0);
    void readDataOfVectors(const QVector<double> &vectorMuA,
                           const QVector<double> &vectorMuForecastA,
                           const QVector<QString> &vectorAlphaA,
                           const QVector<QString> &vectorAlphaForecastA,
                           const QVector<double> &vectorMuB,
                           const QVector<double> &vectorMuForecastB,
                           const QVector<QString> &vectorAlphaB,
                           const QVector<QString> &vectorAlphaForecastB,
                           const QVector<double> &vectorMuC,
                           const QVector<double> &vectorMuForecastC,
                           const QVector<QString> &vectorAlphaC,
                           const QVector<QString> &vectorAlphaForecastC);
    ~LevelTwoChart();
};

#endif // LEVELTWOCHART_H
