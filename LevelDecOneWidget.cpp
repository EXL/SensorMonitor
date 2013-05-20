#include "LevelDecOneWidget.h"

#include <QAction>
#include <qmath.h>

#ifdef _DEBUG
#include <QDebug>
#endif

LevelDecOne::LevelDecOne(const QVector<QString> &vectorDate,
                         const QVector<QVector<double> > &vectorSensorReadings2D,
                         QWidget *parent) :
    QWidget(parent)
{
    str_date = tr("DATE");
    str_mu = tr("Mu");
    str_alpha = tr("Alpha");
    str_forecast = tr("Forecast");

    vectorDateToLevelOne = vectorDate;
    vectorSensorReadings2DToLevelOne = vectorSensorReadings2D;

    row = vectorDateToLevelOne.size();
    col = vectorSensorReadings2DToLevelOne[0].size();

    createTable();

    setVectorsToTable();

    createDoubleBox();

    createChart();

    createToolBar();

    createWidgets();
}

void LevelDecOne::createTable()
{
    viewTableLevelOneModel = new QTableView(this);
    tableLevelOneModel = new TableLevelOneModel(str_date,
                                                str_mu,
                                                str_alpha,
                                                str_forecast,
                                                this);

    viewTableLevelOneModel->setModel(tableLevelOneModel);
}

void LevelDecOne::createDoubleBox()
{
    spbDoubleBox = new QDoubleSpinBox(this);
    spbDoubleBox->setRange(0.1, 1.0);
    spbDoubleBox->setSingleStep(0.1);
    spbDoubleBox->setWrapping(false);
    spbDoubleBox->setToolTip(tr("Please change Coefficient \"A\""));
    connect(spbDoubleBox, SIGNAL(valueChanged(double)), this, SLOT(changedAlpha(double)));
}

void LevelDecOne::createChart()
{
    levelOneChart = new LevelOneChart(this);
    levelOneChart->readDataOfVectors(true,
                                     muVector,
                                     muVectorForecast,
                                     alphaVector,
                                     alphaVectorForecast);
}

void LevelDecOne::createToolBar()
{
    exportLevelOneChart = new QAction(this);
    exportLevelOneChart->setIcon(QIcon("://icons/chart_icons/export_chart_icon.png"));
    exportLevelOneChart->setShortcut(QKeySequence::Print);
    exportLevelOneChart->setText(tr("Export"));
    exportLevelOneChart->setToolTip(tr("Export the Level One Decomposition Chart"));
    connect(exportLevelOneChart, SIGNAL(triggered()), levelOneChart, SLOT(exportLevelOnePlotToImage()));

    printLevelOneChart = new QAction(this);
    printLevelOneChart->setIcon(QIcon("://icons/chart_icons/print_chart_icon.png"));
    printLevelOneChart->setShortcut(QKeySequence::Save);
    printLevelOneChart->setText(tr("Print"));
    printLevelOneChart->setToolTip(tr("Print the Level One Decomposition Chart"));
    connect(printLevelOneChart, SIGNAL(triggered()), levelOneChart, SLOT(printLevelOnePlot()));

    toolBar = new QToolBar(this);
    toolBar->addAction(exportLevelOneChart);
    toolBar->addAction(printLevelOneChart);

    toolBar->addSeparator();

    toolBarBox = new QWidget(toolBar);

    horizToolBarLayout = new QHBoxLayout(toolBarBox);
    horizToolBarLayout->setSpacing(10);

    /* Spacer */
    horizToolBarLayout->addWidget(new QWidget(toolBarBox), 10);

    labelCoeff = new QLabel(toolBarBox);
    labelCoeff->setText(tr("<h3>Coefficient \"A\": </h3>"));

    horizToolBarLayout->addWidget(labelCoeff , 0);
    horizToolBarLayout->addSpacing(10);
    horizToolBarLayout->addWidget(spbDoubleBox, 0);
    toolBar->addWidget(toolBarBox);

    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}

void LevelDecOne::createWidgets()
{
    horizLayoutTableLevelOne = new QHBoxLayout;
    horizLayoutTableLevelOne->addWidget(viewTableLevelOneModel);
    horizLayoutTableLevelOne->addWidget(levelOneChart);

    /* So-slow, do not use splitter in this widget */
    // levelOneSplitter = new QSplitter(this);
    // horizLayoutTableLevelOne->addWidget(levelOneSplitter);

    vertLayoutLevelOne = new QVBoxLayout(this);
    vertLayoutLevelOne->addWidget(toolBar);
    vertLayoutLevelOne->addLayout(horizLayoutTableLevelOne);

    setLayout(vertLayoutLevelOne);
}

void LevelDecOne::setVectorsToTable()
{
    for (size_t i = 0; i < row; ++i)
    {
        muVector.push_back(getMu(i));
        alphaVector.push_back(getAlphaString(i));
    }

    avrg_mu = getAvrgMu();
    avrg_alpha = getAvrgAlpha();

    for (size_t i = 0; i <= row; ++i)
    {
        getMuForecast(i, 0.1);
        getAlphaForecast(i, 0.1);
    }

    tableLevelOneModel->setCurrencyVectors(vectorDateToLevelOne,
                                           muVector,
                                           muVectorForecast,
                                           alphaVector,
                                           alphaVectorForecast);

    viewTableLevelOneModel->selectRow(row - 1);
}

double LevelDecOne::getMu(int i)
{
    double mu = 0;
    for (size_t j = 0; j < col; ++j)
    {
        mu += qPow(vectorSensorReadings2DToLevelOne[i][j], 2);
    }
    return qSqrt(mu);
}

double LevelDecOne::getAvrgMu()
{
    double mu_avrg = 0;

    for (unsigned int i = 0; i < row; ++i)
    {
        mu_avrg += muVector[i];
    }

#ifdef _DEBUG
    qDebug() << "Avgr Mu:" << mu_avrg / row;
#endif

    return mu_avrg / row;
}

QString LevelDecOne::getAlphaString(int i)
{
    double vecMultiplic = 0;
    double muMultiplic = 0;
    if (!i)
    {
        return QString::number(0);
    }
    else
    {
        for (size_t j = 0; j < col; ++j)
        {
            vecMultiplic += vectorSensorReadings2DToLevelOne[0][j] * vectorSensorReadings2DToLevelOne[i][j];
            muMultiplic = getMu(0) * getMu(i);
        }
    }
    return QString::number(qAcos(vecMultiplic/muMultiplic), 'f', 6);
}

double LevelDecOne::getAvrgAlpha()
{
    double alpha_avrg = 0;

    for (size_t i = 0; i < row; ++i)
    {
        alpha_avrg += alphaVector[i].toDouble();
    }

#ifdef _DEBUG
    qDebug() << "Avgr Alpha:" << alpha_avrg / row;
#endif

    return alpha_avrg / row;
}

void LevelDecOne::getMuForecast(int i, double a)
{
    double mu_forecast;

    if (i == 0)
    {
        mu_forecast = muVector[0] * a + (1 - a) * avrg_mu;
    }
    else if (i > 0 && i < row)
    {
        mu_forecast = muVector[i] * a + (1 - a) * muVector[i - 1];
    }
    else if (i == row)
    {
        mu_forecast = avrg_mu * a + (1 - a) * muVector[i - 1];
    }

    muVectorForecast.push_back(mu_forecast);
}

double LevelDecOne::getAvrgMuForecast()
{
    double mu_avrg_forecast = 0;

    for (size_t i = 0; i < row; ++i)
    {
        mu_avrg_forecast += muVectorForecast[i];
    }

#ifdef _DEBUG
    qDebug() << "Avgr Mu Forecast:" << mu_avrg_forecast / row;
#endif

    return mu_avrg_forecast / row;
}

void LevelDecOne::getAlphaForecast(int i, double a)
{
    double alpha_forecast;

    if (i == 0)
    {
        alpha_forecast = alphaVector[0].toDouble() * a + (1 - a) * avrg_alpha;
    }
    else if (i > 0 && i < row)
    {
        alpha_forecast = alphaVector[i].toDouble() * a + (1 - a) * alphaVector[i - 1].toDouble();
    }
    else if (i == row)
    {
        alpha_forecast = avrg_alpha * a + (1 - a) * alphaVector[i - 1].toDouble();
    }

    alphaVectorForecast.push_back(QString::number(alpha_forecast, 'f', 6));
}

double LevelDecOne::getAvrgAlphaForecast()
{
    double alpha_avrg_forecast = 0;

    for (size_t i = 0; i < row; ++i)
    {
        alpha_avrg_forecast += alphaVectorForecast[i].toDouble();
    }

#ifdef _DEBUG
    qDebug() << "Avgr Alpha Forecast:" << alpha_avrg_forecast / row;
#endif

    return alpha_avrg_forecast / row;
}

void LevelDecOne::changedAlpha(double i)
{
    muVectorForecast.clear();
    alphaVectorForecast.clear();

    for (size_t j = 0; j <= row; ++j)
    {
        getMuForecast(j, i);
        getAlphaForecast(j, i);
    }

    tableLevelOneModel->setCurrencyVectors(vectorDateToLevelOne,
                                           muVector,
                                           muVectorForecast,
                                           alphaVector,
                                           alphaVectorForecast);

    viewTableLevelOneModel->selectRow(row - 1);

    // viewTableLevelOneModel->setFocus();
    levelOneChart->readDataOfVectors(false,
                                     muVector,
                                     muVectorForecast,
                                     alphaVector,
                                     alphaVectorForecast);
}

LevelDecOne::~LevelDecOne()
{
    /* Empty Destructor */
}

/************** TABLE MODEL *************/

TableLevelOneModel::TableLevelOneModel(const QString &str_date,
                                       const QString &str_mu,
                                       const QString &str_alpha,
                                       const QString &str_forecast, QObject *parent)
    : QAbstractTableModel(parent)
{
    str_alpha_table = str_alpha;
    str_mu_table = str_mu;
    str_forecast_table = str_forecast;
    str_date_table = str_date;

    lightYellow.setRgb(255, 255, 153);
}

void TableLevelOneModel::setCurrencyVectors(const QVector<QString> &tableVectorDate,
                                            const QVector<double> &muVector,
                                            const QVector<double> &muForecastVector,
                                            const QVector<QString> &alphaVector,
                                            const QVector<QString> &alphaForecastVector)
{
    row_count = muVector.size();

    tableVectorDateOfLevel = tableVectorDate;
    muVectorOfLevel = muVector;
    muForecastVectorOfLevel = muForecastVector;
    alphaVectorOfLevel = alphaVector;
    alphaForecastVectorOfLevel = alphaForecastVector;

    reset();
}

int TableLevelOneModel::rowCount(const QModelIndex &) const
{
    return muForecastVectorOfLevel.size();
}

int TableLevelOneModel::columnCount(const QModelIndex &) const
{
    return 5;
}

QVariant TableLevelOneModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
        case 0:
            return (index.row() < row_count) ? QVariant(tableVectorDateOfLevel[index.row()]) : QVariant(str_forecast_table);
        case 1:
            return (index.row() < row_count) ? QVariant(muVectorOfLevel[index.row()]) : QVariant();
        case 2:
            return (index.row() < row_count) ? QVariant(alphaVectorOfLevel[index.row()]) : QVariant();
        case 3:
            return QVariant(muForecastVectorOfLevel[index.row()]);
        case 4:
            return QVariant(alphaForecastVectorOfLevel[index.row()]);
        default:
            break;
        }
    }
    else if (role == Qt::BackgroundColorRole && index.column() == 0)
    {
        return QVariant(lightYellow);
    }
    return QVariant();
}

QVariant TableLevelOneModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }
    else if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch (section)
        {
        case 0:
            return str_date_table;
        case 1:
            return str_mu_table;
        case 2:
            return str_alpha_table;
        case 3:
            return str_mu_table + " " + str_forecast_table;
        case 4:
            return str_alpha_table + " " + str_forecast_table;
        default:
            break;
        }
    }
    else
    {
        return QString("%1").arg(section + 1);
    }
    return QVariant();
}

TableLevelOneModel::~TableLevelOneModel()
{
    /* Empty Destructor */
}
