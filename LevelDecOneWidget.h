#ifndef LEVELDECONE_H
#define LEVELDECONE_H

#include "LevelOneChart.h"

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

class LevelDecOne : public QWidget
{
    Q_OBJECT

    QString str_date;
    QString str_mu;
    QString str_alpha;
    QString str_forecast;

    QVector<QString> vectorDateToLevelOne;
    QVector<QVector<double> > vectorSensorReadings2DToLevelOne;

    size_t row;
    size_t col;

    double avrg_mu;
    double avrg_alpha;

    QVector<double> muVector;
    QVector<QString> alphaVector;

    QVector<double> muVectorForecast;
    QVector<QString> alphaVectorForecast;

    QDoubleSpinBox *spbDoubleBox;
    QLabel *labelCoeff;

    QHBoxLayout *horizToolBarLayout;
    QHBoxLayout *horizLayoutTableLevelOne;

    // QSplitter *levelOneSplitter;

    QAction *printLevelOneChart;
    QAction *exportLevelOneChart;

    QToolBar *toolBar;
    QWidget *toolBarBox;

    QVBoxLayout *vertLayoutLevelOne;

    TableLevelOneModel *tableLevelOneModel;
    QTableView *viewTableLevelOneModel;

    LevelOneChart *levelOneChart;

    double getMu(int i);
    double getAvrgMu();

    QString getAlphaString(int i);
    double getAvrgAlpha();

    void getMuForecast(int i, double a);
    double getAvrgMuForecast();

    void getAlphaForecast(int i, double a);
    double getAvrgAlphaForecast();

    void createTable();
    void createDoubleBox();
    void createChart();
    void createToolBar();
    void createWidgets();
    void setVectorsToTable();
private slots:
    void changedAlpha(double i);
public:
    LevelDecOne(const QVector<QString> &vectorDate,
                const QVector<QVector<double> > &vectorSensorReadings2D,
                QWidget *parent = 0);
    ~LevelDecOne();
    QHBoxLayout *getLayoutLevelOne() const;
    void setLayoutLevelOne(QHBoxLayout *value);
};

class TableLevelOneModel : public QAbstractTableModel
{
    size_t row_count;

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

    int rowCount(const QModelIndex &) const;
    int columnCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
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

    void setHeaderDataOfTable(const QString *date, const QString *sensor);


    ~TableLevelOneModel();
};

#endif // LEVELDECONE_H
