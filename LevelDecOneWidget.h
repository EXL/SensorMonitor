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
    void getMuForecast(int i, double a);
    void getAlphaForecast(int i, double a);

    QString getAlphaString(int i);
    double getAvrgAlpha();

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
    TableLevelOneModel(QObject *parent = 0);

    void setCurrencyVectors(const QVector<QString> &tableVectorDate,
                            const QVector<double> &muVector,
                            const QVector<double> &muForecastVector,
                            const QVector<QString> &alphaVector,
                            const QVector<QString> &alphaForecastVector);

    void setHeaderDataOfTable(const QString *date, const QString *sensor);


    ~TableLevelOneModel();
};

#endif // LEVELDECONE_H
