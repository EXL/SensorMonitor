#include "LevelDecOneWidget.h"

#include <QAction>
#include <QPushButton>
#include <QMessageBox>
#include <qmath.h>

#ifdef _DEBUG
#include <QDebug>
#endif

LevelDecOne::LevelDecOne(const QVector<QString> &vectorDate,
                         const QVector<QVector<double> > &vectorSensorReadings2D,
                         QWidget *parent) :
    QWidget(parent)
{
    vectorDateToLevelOne = vectorDate;
    vectorSensorReadings2DToLevelOne = vectorSensorReadings2D;

    row = vectorDateToLevelOne.size();
    col = vectorSensorReadings2DToLevelOne[0].size();

    retranslateUi();

    createTables();

    setVectorsToTables();

    createDoubleBoxes();

    createCharts();

    createToolBar();

    createWidgets();
}


void LevelDecOne::retranslateUi()
{
    str_date = tr("DATE");
    str_mu = tr("Mu");
    str_alpha = tr("Alpha");
    str_forecast = tr("Forecast");
    lower_limit = tr("[Low.Lim]");
    upper_limit = tr("[Upp.Lim]");
    lower_limit_short = tr("[L.L]");
    upper_limit_short = tr("[U.L]");
    result_str = tr("Result");
    stable_str = tr("Stable");
    unstable_str = tr("Unstable");
}

void LevelDecOne::createTables()
{
    viewTableLevelOneModel = new QTableView(this);
    tableLevelOneModel = new TableLevelOneModel(str_date,
                                                str_mu,
                                                str_alpha,
                                                str_forecast,
                                                this);

    tableStabilityLevelOneModel = new TableStabilityLevelOneModel(str_date,
                                                                  str_mu,
                                                                  lower_limit,
                                                                  upper_limit,
                                                                  lower_limit_short,
                                                                  upper_limit_short,
                                                                  result_str,
                                                                  stable_str,
                                                                  unstable_str,
                                                                  this);
    viewTableLevelOneModel->setModel(tableLevelOneModel);
}

void LevelDecOne::createDoubleBoxes()
{
    spbDoubleBoxAlpha = new QDoubleSpinBox(this);
    spbDoubleBoxAlpha->setRange(0.1, 1.0);
    spbDoubleBoxAlpha->setSingleStep(0.1);
    spbDoubleBoxAlpha->setWrapping(false);
    spbDoubleBoxAlpha->setToolTip(tr("Please change Coefficient \"A\""));
    connect(spbDoubleBoxAlpha, SIGNAL(valueChanged(double)), this, SLOT(changedAlpha(double)));

    spbDoubleBoxEps = new QwtCounter(this);
    spbDoubleBoxEps->setRange(0.0001, 0.02, 0.0001);
    spbDoubleBoxEps->setValue(0.001);
    spbDoubleBoxEps->setToolTip(tr("Please change Coefficient \"Eps\""));
    connect(spbDoubleBoxEps, SIGNAL(valueChanged(double)), this, SLOT(changedEps(double)));
    connect(spbDoubleBoxEps, SIGNAL(valueChanged(double)), this, SLOT(setDoubleCountToLabel(double)));
}

void LevelDecOne::createCharts()
{
    levelOneChart = new LevelOneChart(this);
    levelOneChart->readDataOfVectors(true,
                                     muVector,
                                     muVectorForecast,
                                     alphaVector,
                                     alphaVectorForecast);

    levelOneMuChart = new LevelOneMuChart(this);
    levelOneMuChart->readDataOfVectors(true,
                                       muLowerLimitVector,
                                       muVector,
                                       muUpperLimitVector,
                                       alphaVector);

}

void LevelDecOne::createToolBar()
{
    exportLevelOneChart = new QAction(this);
    exportLevelOneChart->setIcon(QIcon("://icons/chart_icons/export_chart_icon.png"));
    exportLevelOneChart->setShortcut(QKeySequence::Print);
    exportLevelOneChart->setText(tr("Export"));
    exportLevelOneChart->setToolTip(tr("Export the Level One Decomposition Chart"));
    connect(exportLevelOneChart, SIGNAL(triggered()), this, SLOT(chooseExportCharts()));

    printLevelOneChart = new QAction(this);
    printLevelOneChart->setIcon(QIcon("://icons/chart_icons/print_chart_icon.png"));
    printLevelOneChart->setShortcut(QKeySequence::Save);
    printLevelOneChart->setText(tr("Print"));
    printLevelOneChart->setToolTip(tr("Print the Level One Decomposition Chart"));
    connect(printLevelOneChart, SIGNAL(triggered()), this, SLOT(choosePrintCharts()));

    switchWidgetsLevelOne = new QAction(this);
    switchWidgetsLevelOne->setIcon(QIcon("://icons/others_icons/swap_levels_32x32.png"));
    switchWidgetsLevelOne->setShortcut(Qt::CTRL + Qt::Key_F);
    switchWidgetsLevelOne->setText(tr("Switch"));
    switchWidgetsLevelOne->setToolTip(tr("Switch between charts and tables"));
    connect(switchWidgetsLevelOne, SIGNAL(triggered()), this, SLOT(hideLevelOneWidgets()));

    toolBar = new QToolBar(this);
    toolBar->addAction(exportLevelOneChart);
    toolBar->addAction(printLevelOneChart);

    toolBar->addSeparator();

    toolBar->addAction(switchWidgetsLevelOne);

    toolBar->addSeparator();

    toolBarBox = new QWidget(toolBar);

    horizToolBarLayout = new QHBoxLayout(toolBarBox);
    horizToolBarLayout->setSpacing(10);

    /* Spacer */
    horizToolBarLayout->addWidget(new QWidget(toolBarBox), 10);

    labelCoeffAlpha = new QLabel(toolBarBox);
    labelCoeffAlpha->setText(tr("<h3>Coefficient \"A\": </h3>"));

    labelCoeffEps = new QLabel(toolBarBox);
    labelCoeffEps->setText(tr("<h3>Coefficient \"Eps\": </h3>"));

    labelMeterEps = new QLabel(toolBarBox);
    labelMeterEps->setText(tr("<h3> = 1 mm</h3>"));

    horizToolBarLayout->addWidget(labelCoeffAlpha , 0);
    horizToolBarLayout->addWidget(labelCoeffEps , 0);
    horizToolBarLayout->addSpacing(10);
    horizToolBarLayout->addWidget(spbDoubleBoxAlpha, 0);
    horizToolBarLayout->addWidget(spbDoubleBoxEps, 0);
    horizToolBarLayout->addWidget(labelMeterEps, 0);

    labelCoeffEps->hide();
    spbDoubleBoxEps->hide();
    labelMeterEps->hide();

    toolBar->addWidget(toolBarBox);

    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
}

void LevelDecOne::createWidgets()
{
    labelWarning = new QLabel(this);
    labelWarning->setText(tr("Warning! System is not stable! You need to go to the second level of decomposition or increase the coefficient \"Eps\""));
    labelWarning->setStyleSheet("QLabel { color: #FFFFFF; background-color: #FF0000 }");
    labelWarning->setContentsMargins(2, 2, 2, 2);

    horizLayoutTableLevelOne = new QHBoxLayout;
    horizLayoutTableLevelOne->addWidget(viewTableLevelOneModel);
    horizLayoutTableLevelOne->addWidget(levelOneChart);
    horizLayoutTableLevelOne->addWidget(levelOneMuChart);

    /* So-slow, do not use splitter in this widget */
    // levelOneSplitter = new QSplitter(this);
    // horizLayoutTableLevelOne->addWidget(levelOneSplitter);

    vertLayoutLevelOne = new QVBoxLayout(this);
    vertLayoutLevelOne->addWidget(toolBar);
    vertLayoutLevelOne->addLayout(horizLayoutTableLevelOne);
    vertLayoutLevelOne->addWidget(labelWarning);

    labelWarning->hide();
    levelOneMuChart->hide();

    setLayout(vertLayoutLevelOne);

    qSwitch = true;
}

void LevelDecOne::setVectorsToTables()
{
    for (size_t i = 0; i < row; ++i)
    {
        muVector.push_back(getMu(i));
        alphaVector.push_back(getAlphaString(i));
    }

    createVector2DSensReadingsLow(0.001);
    createVector2DSensReadingsHigh(0.001);

    for (size_t i = 0; i < row; ++i)
    {
        muLowerLimitVector.push_back(getMuLowerLimit(i));
        muUpperLimitVector.push_back(getMuHighLimit(i));
    }

#ifdef _DEBUG
    qDebug() << "Size of mu Low:" << muLowerLimitVector.size();
    qDebug() << "Size of mu High:" << muUpperLimitVector.size();
#endif

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

    tableStabilityLevelOneModel->setCurrencyVectors(vectorDateToLevelOne,
                                                    muLowerLimitVector,
                                                    muVector,
                                                    muUpperLimitVector);

    viewTableLevelOneModel->selectRow(row);
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

void LevelDecOne::getMuForecast(size_t i, double a)
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

void LevelDecOne::getAlphaForecast(size_t i, double a)
{
    double alpha_forecast = 0;

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

void LevelDecOne::createVector2DSensReadingsLow(double eps)
{
    vector2DSensorReadingsLow.clear();

    for (size_t i = 0; i < row; ++i)
    {
        QVector<double> tempVector;
        for (size_t j = 0; j < col; ++j)
        {
            tempVector.push_back(vectorSensorReadings2DToLevelOne[i][j] - eps);
        }
        vector2DSensorReadingsLow.push_back(tempVector);
    }

#ifdef _DEBUG
    qDebug() << "Mu Low[0][0]:" << vector2DSensorReadingsLow[0][0];
#endif
}

void LevelDecOne::createVector2DSensReadingsHigh(double eps)
{
    vector2DSensorReadingsHigh.clear();

    for (size_t i = 0; i < row; ++i)
    {
        QVector<double> tempVector;
        for (size_t j = 0; j < col; ++j)
        {
            tempVector.push_back(vectorSensorReadings2DToLevelOne[i][j] + eps);
        }
        vector2DSensorReadingsHigh.push_back(tempVector);
    }

#ifdef _DEBUG
    qDebug() << "Mu High[0][0]:" << vector2DSensorReadingsHigh[0][0];
#endif
}

double LevelDecOne::getMuLowerLimit(size_t i)
{
    double mu_lower_limit = 0;
    for (size_t j = 0; j < col; ++j)
    {
        mu_lower_limit += qPow(vector2DSensorReadingsLow[i][j], 2);
    }
    return qSqrt(mu_lower_limit);
}

double LevelDecOne::getMuHighLimit(size_t i)
{
    double mu_high_limit = 0;
    for (size_t j = 0; j < col; ++j)
    {
        mu_high_limit += qPow(vector2DSensorReadingsHigh[i][j], 2);
    }
    return qSqrt(mu_high_limit);
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

    viewTableLevelOneModel->selectRow(row);

    // viewTableLevelOneModel->setFocus();
    levelOneChart->readDataOfVectors(false,
                                     muVector,
                                     muVectorForecast,
                                     alphaVector,
                                     alphaVectorForecast);
}

void LevelDecOne::changedEps(double eps)
{
    muLowerLimitVector.clear();
    muUpperLimitVector.clear();

    createVector2DSensReadingsLow(eps);
    createVector2DSensReadingsHigh(eps);

    for (size_t i = 0; i < row; ++i)
    {
        muLowerLimitVector.push_back(getMuLowerLimit(i));
        muUpperLimitVector.push_back(getMuHighLimit(i));
    }

    tableStabilityLevelOneModel->setCurrencyVectors(vectorDateToLevelOne,
                                                    muLowerLimitVector,
                                                    muVector,
                                                    muUpperLimitVector);

    viewTableLevelOneModel->selectRow(row - 1);

    levelOneMuChart->readDataOfVectors(false,
                                       muLowerLimitVector,
                                       muVector,
                                       muUpperLimitVector,
                                       alphaVector);

    if (!tableStabilityLevelOneModel->checkSystemStable())
    {
        labelWarning->show();
    }
    else
    {
        labelWarning->hide();
    }
}

void LevelDecOne::setDoubleCountToLabel(double eps)
{
    labelMeterEps->setText(QString(tr("<h3> = %1 mm</h3>")).arg(eps * 1000));
}

void LevelDecOne::chooseExportCharts()
{
    QMessageBox *pmbx = new QMessageBox(QMessageBox::Information,
                                        tr("Export"),
                                        tr("What is the chart you want to export?"),
                                        QMessageBox::Yes | QMessageBox::No | QMessageBox::Close);

    pmbx->button(QMessageBox::Yes)->setText(tr("Phase Coordinates"));
    pmbx->button(QMessageBox::No)->setText(tr("Available Deviations"));

    int n = pmbx->exec();
    delete pmbx;

    if (n == QMessageBox::Yes)
    {
        levelOneChart->exportLevelOnePlotToImage();
        return;
    }
    else if (n == QMessageBox::No)
    {
        levelOneMuChart->exportLevelOnePlotToImage();
        return;
    }
    else if (n == QMessageBox::Close)
    {
        return;
    }
}

void LevelDecOne::choosePrintCharts()
{
    QMessageBox *pmbx = new QMessageBox(QMessageBox::Information,
                                        tr("Print"),
                                        tr("What is the chart you want to print?"),
                                        QMessageBox::Yes | QMessageBox::No | QMessageBox::Close);

    pmbx->button(QMessageBox::Yes)->setText(tr("Phase Coordinates"));
    pmbx->button(QMessageBox::No)->setText(tr("Available Deviations"));

    int n = pmbx->exec();
    delete pmbx;

    if (n == QMessageBox::Yes)
    {
        levelOneChart->printLevelOnePlot();
        return;
    }
    else if (n == QMessageBox::No)
    {
        levelOneMuChart->printLevelOnePlot();
        return;
    }
    else if (n == QMessageBox::Close)
    {
        return;
    }
}

void LevelDecOne::hideLevelOneWidgets()
{
    if (qSwitch)
    {
        viewTableLevelOneModel->setModel(tableStabilityLevelOneModel);
        levelOneChart->hide();

        labelCoeffAlpha->hide();
        spbDoubleBoxAlpha->hide();


        labelCoeffEps->show();
        spbDoubleBoxEps->show();
        labelMeterEps->show();
        levelOneMuChart->show();

        qSwitch = false;
    }
    else
    {
        viewTableLevelOneModel->setModel(tableLevelOneModel);
        levelOneMuChart->hide();

        labelCoeffEps->hide();
        spbDoubleBoxEps->hide();
        labelMeterEps->hide();

        levelOneChart->show();
        labelCoeffAlpha->show();
        spbDoubleBoxAlpha->show();

        qSwitch = true;
    }
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
    generator = new Generators();
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
            return (index.row() < row_count) ?
                        QVariant(tableVectorDateOfLevel[index.row()]) :
                    QVariant(str_forecast_table + " (" + generator->dateGenerator() + ")");
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

/************** END TABLE MODEL *************/

/************** TABLE MODEL #2 *************/

TableStabilityLevelOneModel::TableStabilityLevelOneModel(const QString &str_date,
                                                         const QString &str_mu,
                                                         const QString &lower_limit,
                                                         const QString &upper_limit,
                                                         const QString &lower_limit_short,
                                                         const QString &upper_limit_short,
                                                         const QString &result_str,
                                                         const QString &stable_str,
                                                         const QString &unstable_str,
                                                         QObject *parent)
    : QAbstractTableModel(parent)
{
    str_date_table = str_date;
    str_mu_table = str_mu;
    str_lower_limit = lower_limit;
    str_upper_limit = upper_limit;
    str_lower_limit_short = lower_limit_short;
    str_upper_limit_short = upper_limit_short;
    str_result = result_str;
    str_stable = stable_str;
    str_unstable = unstable_str;

    lightYellow.setRgb(255, 255, 153);
}

void TableStabilityLevelOneModel::setCurrencyVectors(const QVector<QString> &tableVectorDate,
                                                     const QVector<double> &muLowerLimitVector,
                                                     const QVector<double> &muVector,
                                                     const QVector<double> &muUpperLimitVector)
{
    row_count = muVector.size();

    tableVectorDateOfLevel = tableVectorDate;
    muLowerLimitVectorOfLevel = muLowerLimitVector;
    muVectorOfLevel = muVector;
    muUpperLimitVectorOfLevel = muUpperLimitVector;

    reset();
}

int TableStabilityLevelOneModel::rowCount(const QModelIndex &) const
{
    return muVectorOfLevel.size();
}

int TableStabilityLevelOneModel::columnCount(const QModelIndex &) const
{
    return 7;
}

QVariant TableStabilityLevelOneModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
        case 0:
            return QVariant(tableVectorDateOfLevel[index.row()]);
        case 1:
            return QVariant(muLowerLimitVectorOfLevel[index.row()]);
        case 2:
            return QVariant(muVectorOfLevel[index.row()]);
        case 3:
            return QVariant(muUpperLimitVectorOfLevel[index.row()]);
        case 4:
            return QVariant(muUpperLimitVectorOfLevel[index.row()] - muLowerLimitVectorOfLevel[index.row()]);
        case 5:
            return (!index.row()) ?
                        QVariant(0) :
                        QVariant(muVectorOfLevel[index.row()] - muVectorOfLevel[0]);
        case 6:
            return (checkResult(index.row())) ?
                        QVariant(str_stable) :
                        QVariant(str_unstable);
        default:
            break;
        }
    }
    else if (role == Qt::BackgroundColorRole && index.column() == 0)
    {
        return QVariant(lightYellow);
    }
    else if (role == Qt::BackgroundColorRole && index.column() == 6)
    {
        if (checkResult(index.row()))
        {
            return QVariant(Qt::green);
        }
        else
        {
            return QVariant(Qt::red);
        }
    }
    return QVariant();
}

QVariant TableStabilityLevelOneModel::headerData(int section, Qt::Orientation orientation, int role) const
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
            return str_mu_table + str_lower_limit;
        case 2:
            return str_mu_table;
        case 3:
            return str_mu_table + str_upper_limit;
        case 4:
            return str_mu_table + str_upper_limit_short + "-" + str_mu_table + str_lower_limit_short;
        case 5:
            return str_mu_table + "[i]-" + str_mu_table + "[0]";
        case 6:
            return str_result;
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

bool TableStabilityLevelOneModel::checkResult(int i) const
{
    bool check_result = (muUpperLimitVectorOfLevel[i] - muLowerLimitVectorOfLevel[i]) >=
            (muVectorOfLevel[i] - muVectorOfLevel[0]);

    if (check_result)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool TableStabilityLevelOneModel::checkSystemStable() const
{
    QVector<bool> checkSystemStableVector;

    for (size_t i = 0; i < row_count; ++i)
    {
        checkSystemStableVector.push_back(checkResult(i));
    }

    for (size_t j = 0; j < row_count; ++j)
    {
        if (!checkSystemStableVector[j])
        {
            return false;
        }
    }
    return true;
}

TableStabilityLevelOneModel::~TableStabilityLevelOneModel()
{
    /* Empty Destructor */
}

/************** END TABLE MODEL #2 *************/
