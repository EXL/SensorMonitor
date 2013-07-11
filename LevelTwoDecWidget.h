#ifndef LEVELTWODECWIDGET_H
#define LEVELTWODECWIDGET_H

#include "LevelTwoChart.h"
#include "LevelTwoMuChart.h"
#include "Generators.h"

#include <qwt_counter.h>

#include <QWidget>
#include <QAbstractTableModel>
#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolBar>
#include <QLabel>
#include <QDoubleSpinBox>

class TableLevelTwoModel;
class TableStabilityLevelTwoModel;

struct structHTMLReportLevelTwo
{
    QString header;
    QString tableOne;
    QString tableTwo;
    QString footer;
    QString tdOpen;
    QString tdClose;
    QString trOpen;
    QString trClose;
};

class LevelTwoDec : public QWidget
{
    Q_OBJECT

    size_t row;
    size_t col;

    bool qSwitch;

    QString str_date;
    QString str_mu;
    QString str_alpha;
    QString str_forecast;
    QString lower_limit;
    QString upper_limit;
    QString lower_limit_short;
    QString upper_limit_short;
    QString result_str;
    QString stable_str;
    QString unstable_str;

    double avrg_mu_blockA;
    double avrg_mu_blockB;
    double avrg_mu_blockC;
    double avrg_alpha_blockA;
    double avrg_alpha_blockB;
    double avrg_alpha_blockC;

    /********** FIRST TABLE **********/
    QVector<QString> vectorDateToLevelTwo;
    QVector<QVector<double> > vectorBlockAToLevelTwo;
    QVector<QVector<double> > vectorBlockBToLevelTwo;
    QVector<QVector<double> > vectorBlockCToLevelTwo;

    QVector<double> vectorMuBlockA;
    QVector<double> vectorMuBlockB;
    QVector<double> vectorMuBlockC;
    QVector<QString> vectorAlphaBlockA;
    QVector<QString> vectorAlphaBlockB;
    QVector<QString> vectorAlphaBlockC;

    QVector<double> vectorForecastMuBlockA;
    QVector<double> vectorForecastMuBlockB;
    QVector<double> vectorForecastMuBlockC;
    QVector<QString> vectorForecastAlphaBlockA;
    QVector<QString> vectorForecastAlphaBlockB;
    QVector<QString> vectorForecastAlphaBlockC;
    /********** END FIRST TABLE **********/

    /********** SECOND TABLE **********/
    QVector<QVector<double> > vectorBlockALow;
    QVector<QVector<double> > vectorBlockAHigh;
    QVector<QVector<double> > vectorBlockBLow;
    QVector<QVector<double> > vectorBlockBHigh;
    QVector<QVector<double> > vectorBlockCLow;
    QVector<QVector<double> > vectorBlockCHigh;

    QVector<double> vectorMuBlockALowerLimit;
    QVector<double> vectorMuBlockAUpperLimit;
    QVector<double> vectorMuBlockBLowerLimit;
    QVector<double> vectorMuBlockBUpperLimit;
    QVector<double> vectorMuBlockCLowerLimit;
    QVector<double> vectorMuBlockCUpperLimit;
    /********** END SECOND TABLE **********/

    QTableView *viewTableLevelTwoModel;
    TableLevelTwoModel *tableLevelTwoModel;
    TableStabilityLevelTwoModel *tableStabilityLevelTwoModel;

    QHBoxLayout *horizLayoutTableLevelTwo;
    QVBoxLayout *vertLayoutTableLevelTwo;

    QToolBar *toolBar;
    QWidget *toolBarBox;
    QHBoxLayout *horizToolBarLayout;

    QLabel *labelCoeffEps;
    QLabel *labelMeterEps;
    QLabel *labelCoeffAlpha;
    QLabel *labelWarning;
    QLabel *blocksToolTips;

    QAction *exportLevelTwoChart;
    QAction *printLevelTwoChart;
    QAction *exportReportLevelTwo;
    QAction *switchWidgetsLevelTwo;
    QAction *showHelpInBrowserAction;

    QDoubleSpinBox *spbDoubleBoxAlpha;
    QwtCounter *spbDoubleBoxEps;

    LevelTwoChart *levelTwoChart;
    LevelTwoMuChart *levelTwoMuChart;

    void splitVectors(const QVector<QVector<double> > &vectorSensorReadings2D);
    void setVectorsToTables();

    /********** FIRST TABLE **********/
    double getMu(unsigned int i,
                 const QVector<QVector<double> > &vectorTarget2D);
    double getAvrgMu(const QVector<double> &vectorTarget);
    double getAvrgForecastMu(const QVector<double> &vectorTarget);

    QString getAlphaString(unsigned int i,
                           const QVector<QVector<double> > &vectorTarget2D);
    double getAvrgAlpha(const QVector<QString> &vectorTarget);
    double getAvrgForecastAlpha(const QVector<QString> &vectorTarget);

    void getMuForecast(size_t i, double a, double avrg_mu,
                       const QVector<double> &vectorTarget,
                       QVector<double> &vectorOutput);

    void getAlphaForecast(size_t i, double a, double avrg_alpha,
                          const QVector<QString> &vectorTarget,
                          QVector<QString> &vectorOutput);
    double ArcCos(double x);
    /********** END FIRST TABLE **********/

    /********** SECOND TABLE **********/
    void createBlocksVectorsSensReadingsLandH(bool qLow,
                                              double eps,
                                              const QVector<QVector<double> > &vectorTarget,
                                              QVector<QVector<double> > &vectorOutput);
    double getMuLimits(size_t i, QVector<QVector<double> > &vectorTarget);
    /********** END SECOND TABLE **********/

    void retranslateUi();
    void createTables();
    void readDataOfVectorsToChart();
    void readDataOfVectorsToMuChart();
    void createDoubleBoxes();
    void createCharts();
    void createToolBar();
    void createWidgets();
    void checkStability();
private slots:
    void changedAlpha(double i);
    void changedEps(double eps);

    void setDoubleCountToLabel(double eps);

    void chooseExportCharts();
    void choosePrintCharts();

    void hideLevelTwoWidgets();

    void exportReportToHTML();
    void showHelp();
public:
    LevelTwoDec(const QVector<QString> &vectorDate,
                const QVector<QVector<double> > &vectorSensorReadings2D,
                QWidget *parent = 0);
    ~LevelTwoDec();
};

class TableLevelTwoModel : public QAbstractTableModel
{
    int row_count;

    QString str_date_table;
    QString str_mu_table;
    QString str_alpha_table;
    QString str_forecast_table;
    QString str_blockA;
    QString str_blockB;
    QString str_blockC;

    QColor lightYellow;
    QColor lightRed;
    QColor lightGreen;
    QColor lightBlue;

    QVector<QString> tableVectorDateOfLevel;

    QVector<double> muBlockAVectorOfLevel;
    QVector<double> muBlockBVectorOfLevel;
    QVector<double> muBlockCVectorOfLevel;
    QVector<double> muBlockAForecastVectorOfLevel;
    QVector<double> muBlockBForecastVectorOfLevel;
    QVector<double> muBlockCForecastVectorOfLevel;
    QVector<QString> alphaBlockAVectorOfLevel;
    QVector<QString> alphaBlockBVectorOfLevel;
    QVector<QString> alphaBlockCVectorOfLevel;
    QVector<QString> alphaBlockAForecastVectorOfLevel;
    QVector<QString> alphaBlockBForecastVectorOfLevel;
    QVector<QString> alphaBlockCForecastVectorOfLevel;

    Generators *generator;
protected:
    int rowCount(const QModelIndex &) const;
    int columnCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
public:
    TableLevelTwoModel(const QString &str_date,
                       const QString &str_mu,
                       const QString &str_alpha,
                       const QString &str_forecast,
                       QObject *parent = 0);
    void setCurrencyVectors(const QVector<QString> &tableVectorDate,
                            const QVector<double> &vectorMuBlockA,
                            const QVector<double> &vectorForecastMuBlockA,
                            const QVector<QString> &vectorAlphaBlockA,
                            const QVector<QString> &vectorForecastAlphaBlockA,
                            const QVector<double> &vectorMuBlockB,
                            const QVector<double> &vectorForecastMuBlockB,
                            const QVector<QString> &vectorAlphaBlockB,
                            const QVector<QString> &vectorForecastAlphaBlockB,
                            const QVector<double> &vectorMuBlockC,
                            const QVector<double> &vectorForecastMuBlockC,
                            const QVector<QString> &vectorAlphaBlockC,
                            const QVector<QString> &vectorForecastAlphaBlockC);
    ~TableLevelTwoModel();
};

class TableStabilityLevelTwoModel : public QAbstractTableModel
{
    size_t row_count;

    QString str_date_table;
    QString str_lower_limit;
    QString str_upper_limit;
    QString str_lower_limit_short;
    QString str_upper_limit_short;
    QString str_mu_table;
    QString str_result;
    QString str_stable;
    QString str_unstable;
    QString str_blockA;
    QString str_blockB;
    QString str_blockC;

    QColor lightYellow;
    QColor lightRed;
    QColor lightGreen;
    QColor lightBlue;

    QVector<QString> tableVectorDateOfLevel;

    QVector<double> muBlockALLVectorOfLevel;
    QVector<double> muBlockAVectorOfLevel;
    QVector<double> muBlockAULVectorOfLevel;
    QVector<double> muBlockBLLVectorOfLevel;
    QVector<double> muBlockBVectorOfLevel;
    QVector<double> muBlockBULVectorOfLevel;
    QVector<double> muBlockCLLVectorOfLevel;
    QVector<double> muBlockCVectorOfLevel;
    QVector<double> muBlockCULVectorOfLevel;

    bool checkResult(int i,
                     const QVector<double> &vectorLower,
                     const QVector<double> &vectorMiddle,
                     const QVector<double> &vectorUpper) const;
protected:
    int rowCount(const QModelIndex &) const;
    int columnCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
public:
    TableStabilityLevelTwoModel(const QString &str_date,
                                const QString &str_mu,
                                const QString &lower_limit,
                                const QString &upper_limit,
                                const QString &lower_limit_short,
                                const QString &upper_limit_short,
                                const QString &result_str,
                                const QString &stable_str,
                                const QString &unstable_str,
                                QObject *parent = 0);
    void setCurrencyVectors(const QVector<QString> &tableVectorDate,
                            QVector<double> &vectorMuBlockALowerLimit,
                            QVector<double> &vectorMuBlockA,
                            QVector<double> &vectorMuBlockAUpperLimit,
                            QVector<double> &vectorMuBlockBLowerLimit,
                            QVector<double> &vectorMuBlockB,
                            QVector<double> &vectorMuBlockBUpperLimit,
                            QVector<double> &vectorMuBlockCLowerLimit,
                            QVector<double> &vectorMuBlockC,
                            QVector<double> &vectorMuBlockCUpperLimit);
    bool checkSystemStable() const;
    ~TableStabilityLevelTwoModel();
};

#endif // LEVELTWODECWIDGET_H
