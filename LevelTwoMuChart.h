#ifndef LEVELTWOMUCHART_H
#define LEVELTWOMUCHART_H

#include <qwt_plot.h>
#include <qwt_plot_picker.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>

class LevelTwoMuChart : public QwtPlot
{
    Q_OBJECT

    QwtPlotPicker *plotPicker;
    QwtLegend *legend;
    QwtPlotGrid *gridToPlot;

    /* Markers */
    QwtPlotMarker *mY;
    QwtPlotMarker *mX;

    /********** CURVES **********/
    QwtPlotCurve *curveMuLowerBlockA;
    QwtPlotCurve *curveMuBlockA;
    QwtPlotCurve *curveMuUpperBlockA;

    QwtPlotCurve *curveMuLowerBlockB;
    QwtPlotCurve *curveMuBlockB;
    QwtPlotCurve *curveMuUpperBlockB;

    QwtPlotCurve *curveMuLowerBlockC;
    QwtPlotCurve *curveMuBlockC;
    QwtPlotCurve *curveMuUpperBlockC;
    /********** END CURVES **********/

    /********** SYMBOLS **********/
    QwtSymbol *symbolMuLowerBlockA;
    QwtSymbol *symbolMuBlockA;
    QwtSymbol *symbolMuUpperBlockA;

    QwtSymbol *symbolMuLowerBlockB;
    QwtSymbol *symbolMuBlockB;
    QwtSymbol *symbolMuUpperBlockB;

    QwtSymbol *symbolMuLowerBlockC;
    QwtSymbol *symbolMuBlockC;
    QwtSymbol *symbolMuUpperBlockC;
    /********** END SYMBOLS **********/

    void updateWidgetGradient();
    void plotAssay();
protected:
    virtual void resizeEvent(QResizeEvent *);
private slots:
    void showItem(QwtPlotItem *item, bool on);
public slots:
    void exportLevelTwoPlotToImage();
    void printLevelTwoPlot();
public:
    LevelTwoMuChart(QWidget *parent = 0);
    void readDataOfVectors(const QVector<QString> &vectorAlpha,
                           const QVector<double> &vectorMuLowerBlockA,
                           const QVector<double> &vectorMuBlockA,
                           const QVector<double> &vectorMuUpperBlockA,
                           const QVector<double> &vectorMuLowerBlockB,
                           const QVector<double> &vectorMuBlockB,
                           const QVector<double> &vectorMuUpperBlockB,
                           const QVector<double> &vectorMuLowerBlockC,
                           const QVector<double> &vectorMuBlockC,
                           const QVector<double> &vectorMuUpperBlockC);
    ~LevelTwoMuChart();
};

#endif // LEVELTWOMUCHART_H
