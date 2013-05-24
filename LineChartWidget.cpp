#include "LineChartWidget.h"

#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>

LineChartWidget::LineChartWidget(const QVector<QString> &vectorDate, const QVector<QVector<double> > &vectorSensorReadings2D, QWidget *parent)
    : QMainWindow(parent)
{
    lineChart = new LineChart(vectorDate, vectorSensorReadings2D);

    createActions();

    createMenus();

    createToolBars();

    createCentralWidget();

    createStatusBar();

    settingsWidget();

    retranslateUi();
}

void LineChartWidget::createActions()
{
    exportPlotToImageAction = new QAction(this);
    exportPlotToImageAction->setIcon(QIcon("://icons/chart_icons/export_chart_icon.png"));
    exportPlotToImageAction->setShortcut(QKeySequence::Save);
    connect(exportPlotToImageAction, SIGNAL(triggered()), lineChart, SLOT(exportPlotToImage()));

    printChartAction = new QAction(this);
    printChartAction->setIcon(QIcon("://icons/chart_icons/print_chart_icon.png"));
    printChartAction->setShortcut(QKeySequence::Print);
    connect(printChartAction, SIGNAL(triggered()), lineChart, SLOT(printPlot()));

    closeChartWindowAction = new QAction(this);
    closeChartWindowAction->setIcon(QIcon("://icons/chart_icons/close_chart_icon.png"));
    closeChartWindowAction->setShortcut(Qt::Key_F10);
    connect(closeChartWindowAction, SIGNAL(triggered()), this, SLOT(close()));
}

void LineChartWidget::createMenus()
{
    fileChartMenu = new QMenu(this);
    fileChartMenu->addAction(exportPlotToImageAction);
    fileChartMenu->addAction(printChartAction);

    fileChartMenu->addSeparator();

    fileChartMenu->addAction(closeChartWindowAction);

    menuBar()->setStyleSheet("QMenuBar {"
                             "background-color: #0099CC "
                             "}"

                             "QMenuBar::item {"
                             "color: #FFFFFF;"
                             "background-color: #0099CC"
                             "}"

                             "QMenuBar::item:selected {"
                             "color: #000000;"
                             "background: orange"
                             "}"

                             "QMenuBar::item:pressed {"
                             "color: #000000;"
                             "background: #FFCC33"
                             "}");

    menuBar()->addMenu(fileChartMenu);
}

void LineChartWidget::createToolBars()
{
    chartToolBar = new QToolBar(this);
    chartToolBar->addAction(exportPlotToImageAction);
    chartToolBar->addAction(printChartAction);

    chartToolBar->addSeparator();

    chartToolBar->addAction(closeChartWindowAction);

    chartToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    chartToolBar->setStyleSheet("QToolBar {background-color: #D9F1FF}");

    addToolBar(Qt::LeftToolBarArea, chartToolBar);
}

void LineChartWidget::createCentralWidget()
{
    centralPlotWidget = new QWidget(this);

    layToCentralWidget = new QVBoxLayout(centralPlotWidget);

    tabPlotWidget = new QTabWidget(centralPlotWidget);

    layToCentralWidget->addWidget(tabPlotWidget);

    tabPlotWidget->addTab(lineChart, tr("Chart"));

    centralPlotWidget->setLayout(layToCentralWidget);

    setCentralWidget(centralPlotWidget);
}

void LineChartWidget::createStatusBar()
{
    statusBar()->setStyleSheet("QStatusBar { color: #FFFFFF; background-color: #0099CC }");
}

void LineChartWidget::settingsWidget()
{
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_ShowModal);

    resize(1024, 650);
    setMinimumSize(800, 480);
}

void LineChartWidget::retranslateUi()
{
    exportPlotToImageAction->setText(tr("&Export"));
    exportPlotToImageAction->setStatusTip(tr("Export current chart to various image formats, like as PNG or PDF"));

    printChartAction->setText(tr("&Print"));
    printChartAction->setStatusTip(tr("Send current chart to printer"));

    closeChartWindowAction->setText(tr("&Close"));
    closeChartWindowAction->setStatusTip(tr("Close Chart window"));

    fileChartMenu->setTitle(tr("&File"));
    fileChartMenu->setToolTip(tr("File Menu"));

    chartToolBar->setWindowTitle(tr("Chart ToolBar"));

    lineChart->setStatusTip(tr("Curves sensors"));

    statusBar()->showMessage(tr("Ready"));
    setWindowTitle(tr("LineChart of Sensors Readings"));
}

LineChartWidget::~LineChartWidget()
{
    /* Empty Destructor */
}
