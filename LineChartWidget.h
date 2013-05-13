#ifndef LINECHARTWIDGET_H
#define LINECHARTWIDGET_H

#include "LineChart.h"

#include <QMainWindow>
#include <QVBoxLayout>

class LineChartWidget : public QMainWindow
{
    Q_OBJECT

    LineChart *lineChart;

    QAction *exportPlotToImageAction;
    QAction *printChartAction;
    QAction *closeChartWindowAction;

    QMenu *fileChartMenu;

    QToolBar *chartToolBar;

    QVBoxLayout *layToCentralWidget;
    QWidget *centralPlotWidget;
    QTabWidget *tabPlotWidget;

    void createActions();
    void createMenus();
    void createToolBars();
    void createCentralWidget();
    void createStatusBar();
    void settingsWidget();
    void retranslateUi();
public:
    LineChartWidget(const QVector<QString> &vectorDate, const QVector<QVector<double> > &vectorSensorReadings2D, QWidget *parent = 0);
    ~LineChartWidget();
};

#endif // LINECHARTWIDGET_H
