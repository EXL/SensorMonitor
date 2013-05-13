#include "LevelDecOneWidget.h"

#include <QAction>
#include <qmath.h>

#ifdef _DEBUG
#include <QDebug>
#endif

LevelDecOne::LevelDecOne(const QVector<QString> &vectorDate, const QVector<QVector<double> > &vectorSensorReadings2D, QWidget *parent) :
    QWidget(parent)
{
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
    tableLevelOneModel = new TableLevelOneModel(this);

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

    for (size_t i = 0; i < row; ++i)
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
    qDebug() << mu_avrg / row;
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
    qDebug() << alpha_avrg / row;
#endif
    return alpha_avrg / row;
}

void LevelDecOne::getMuForecast(int i, double a)
{
    double mu_forecast;

    mu_forecast = muVector[i] * a + (1 - a) * avrg_mu;

    muVectorForecast.push_back(mu_forecast);
}

void LevelDecOne::getAlphaForecast(int i, double a)
{
    double alpha_forecast;

    alpha_forecast = alphaVector[i].toDouble() * a + (1 - a) * avrg_alpha;

    alphaVectorForecast.push_back(QString::number(alpha_forecast, 'f', 6));
}

void LevelDecOne::changedAlpha(double i)
{
    muVectorForecast.clear();
    alphaVectorForecast.clear();

    for (size_t j = 0; j < row; ++j)
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

TableLevelOneModel::TableLevelOneModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    lightYellow.setRgb(255, 255, 153);
}

void TableLevelOneModel::setCurrencyVectors(const QVector<QString> &tableVectorDate,
                                            const QVector<double> &muVector,
                                            const QVector<double> &muForecastVector,
                                            const QVector<QString> &alphaVector,
                                            const QVector<QString> &alphaForecastVector)
{
    tableVectorDateOfLevel = tableVectorDate;
    muVectorOfLevel = muVector;
    muForecastVectorOfLevel = muForecastVector;
    alphaVectorOfLevel = alphaVector;
    alphaForecastVectorOfLevel = alphaForecastVector;

    reset();
}

int TableLevelOneModel::rowCount(const QModelIndex &) const
{
    return tableVectorDateOfLevel.size();
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
            return QVariant(tableVectorDateOfLevel[index.row()]);
        case 1:
            return QVariant(muVectorOfLevel[index.row()]);
        case 2:
            return QVariant(muForecastVectorOfLevel[index.row()]);
        case 3:
            return QVariant(alphaVectorOfLevel[index.row()]);
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
            return QString(tr("DATE"));
        case 1:
            return QString(tr("Mu"));
        case 2:
            return QString(tr("Mu Forecast"));
        case 3:
            return QString(tr("Alpha"));
        case 4:
            return QString(tr("Alpha Forecast"));
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
