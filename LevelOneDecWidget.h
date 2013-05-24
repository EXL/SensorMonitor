#ifndef LEVELDECONE_H
#define LEVELDECONE_H

#include "LevelOneChart.h"
#include "LevelOneMuChart.h"
#include "Generators.h"

#include <qwt_counter.h>

#include <QWidget>
#include <QAbstractTableModel>
#include <QTableView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDoubleSpinBox>
#include <QToolBar>
#include <QLabel>

/* For Splitter */
// #include <QSplitter>

class TableLevelOneModel;
class TableStabilityLevelOneModel;

class LevelOneDec : public QWidget
{
    Q_OBJECT

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

    QVector<QString> vectorDateToLevelOne;
    QVector<QVector<double> > vectorSensorReadings2DToLevelOne;

    size_t row;
    size_t col;

    double avrg_mu;
    double avrg_alpha;

    bool qSwitch;

    /********** FIRST TABLE **********/
    QVector<double> muVector;
    QVector<QString> alphaVector;

    QVector<double> muVectorForecast;
    QVector<QString> alphaVectorForecast;
    /********** END FIRST TABLE **********/

    /********** SECOND TABLE **********/
    QVector<QVector<double> > vector2DSensorReadingsLow;
    QVector<QVector<double> > vector2DSensorReadingsHigh;

    QVector<double> muLowerLimitVector;
    QVector<double> muUpperLimitVector;
    /********** END SECOND TABLE **********/

    QDoubleSpinBox *spbDoubleBoxAlpha;
    // QDoubleSpinBox *spbDoubleBoxEps;
    QwtCounter *spbDoubleBoxEps;

    QLabel *labelCoeffAlpha;
    QLabel *labelCoeffEps;
    QLabel *labelMeterEps;
    QLabel *labelWarning;

    QHBoxLayout *horizToolBarLayout;
    QHBoxLayout *horizLayoutTableLevelOne;

    // QSplitter *levelOneSplitter;

    QAction *exportLevelOneChart;
    QAction *printLevelOneChart;
    QAction *switchWidgetsLevelOne;

    QToolBar *toolBar;
    QWidget *toolBarBox;

    QVBoxLayout *vertLayoutLevelOne;

    TableLevelOneModel *tableLevelOneModel;
    TableStabilityLevelOneModel *tableStabilityLevelOneModel;
    QTableView *viewTableLevelOneModel;

    LevelOneChart *levelOneChart;
    LevelOneMuChart * levelOneMuChart;
    /********** FIRST TABLE **********/
    double getMu(int i);
    double getAvrgMu();

    QString getAlphaString(int i);
    double getAvrgAlpha();

    void getMuForecast(size_t i, double a);
    double getAvrgMuForecast();

    void getAlphaForecast(size_t i, double a);
    double getAvrgAlphaForecast();
    /********** END FIRST TABLE **********/

    /********** SECOND TABLE **********/
    void createVector2DSensReadingsLow(double eps);
    void createVector2DSensReadingsHigh(double eps);

    double getMuLowerLimit(size_t i);
    double getMuHighLimit(size_t i);

    /********** END SECOND TABLE **********/
    void retranslateUi();
    void createTables();
    void setVectorsToTables();
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

    void hideLevelOneWidgets();
public:
    LevelOneDec(const QVector<QString> &vectorDate,
                const QVector<QVector<double> > &vectorSensorReadings2D,
                QWidget *parent = 0);
    ~LevelOneDec();
};

class TableLevelOneModel : public QAbstractTableModel
{
    int row_count;

    QString str_date_table;
    QString str_mu_table;
    QString str_alpha_table;
    QString str_forecast_table;

    QColor lightYellow;

    QVector<QString> tableVectorDateOfLevel;

    QVector<double> muVectorOfLevel;
    QVector<double> muForecastVectorOfLevel;

    QVector<QString> alphaVectorOfLevel;
    QVector<QString> alphaForecastVectorOfLevel;

    QVector<QVector<double> > sensorReadingsOfReceived2DVector;

    Generators *generator;

protected:
    int rowCount(const QModelIndex &) const;
    int columnCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role) const;

public:
    TableLevelOneModel(const QString &str_date,
                       const QString &str_mu,
                       const QString &str_alpha,
                       const QString &str_forecast,
                       QObject *parent = 0);

    void setCurrencyVectors(const QVector<QString> &tableVectorDate,
                            const QVector<double> &muVector,
                            const QVector<double> &muForecastVector,
                            const QVector<QString> &alphaVector,
                            const QVector<QString> &alphaForecastVector);
    ~TableLevelOneModel();
};

class TableStabilityLevelOneModel : public QAbstractTableModel
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

    QColor lightYellow;

    QVector<QString> tableVectorDateOfLevel;

    QVector<double> muLowerLimitVectorOfLevel;
    QVector<double> muVectorOfLevel;
    QVector<double> muUpperLimitVectorOfLevel;

    bool checkResult(int i) const;
protected:
    int rowCount(const QModelIndex &) const;
    int columnCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role) const;

public:
    TableStabilityLevelOneModel(const QString &str_date,
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
                            const QVector<double> &muLowerLimitVector,
                            const QVector<double> &muVector,
                            const QVector<double> &muUpperLimitVector);
    bool checkSystemStable() const;
    ~TableStabilityLevelOneModel();
};

#endif // LEVELDECONE_H
