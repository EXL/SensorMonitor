#include "LevelTwoDecWidget.h"

#include <QAction>
#include <QPushButton>
#include <QMessageBox>
#include <qmath.h>

#ifdef _DEBUG
#include <QDebug>
#endif

LevelTwoDec::LevelTwoDec(const QVector<QString> &vectorDate,
                         const QVector<QVector<double> > &vectorSensorReadings2D,
                         QWidget *parent) :
    QWidget(parent)
{
    vectorDateToLevelTwo = vectorDate;

    row = vectorDateToLevelTwo.size();
    col = vectorSensorReadings2D[0].size();

    retranslateUi();

    splitVectors(vectorSensorReadings2D);

    createTables();

    setVectorsToTables();

    createDoubleBoxes();

    createCharts();

    createToolBar();

    createWidgets();
}

void LevelTwoDec::splitVectors(const QVector<QVector<double> > &vectorSensorReadings2D)
{
    for (size_t i = 0; i < row; ++i)
    {
        QVector<double> vectorTempA;
        QVector<double> vectorTempB;
        QVector<double> vectorTempC;

        for (size_t j = 0; j < col; ++j)
        {
            switch (j) {
            case 0:
            case 1:
            case 2:
            case 8:
            case 9:
            case 10:
                vectorTempA.push_back(vectorSensorReadings2D[i][j]);
                break;
            case 3:
            case 4:
            case 5:
            case 11:
            case 12:
            case 13:
                vectorTempB.push_back(vectorSensorReadings2D[i][j]);
                break;
            case 6:
            case 7:
            case 14:
            case 15:
                vectorTempC.push_back(vectorSensorReadings2D[i][j]);
                break;
            default:
                break;
            }
        }

        vectorBlockAToLevelTwo.push_back(vectorTempA);
        vectorBlockBToLevelTwo.push_back(vectorTempB);
        vectorBlockCToLevelTwo.push_back(vectorTempC);
    }
}

void LevelTwoDec::setVectorsToTables()
{
    for (size_t i = 0; i < row; ++i)
    {
        vectorMuBlockA.push_back(getMu(i, vectorBlockAToLevelTwo));
        vectorMuBlockB.push_back(getMu(i, vectorBlockBToLevelTwo));
        vectorMuBlockC.push_back(getMu(i, vectorBlockCToLevelTwo));

        vectorAlphaBlockA.push_back(getAlphaString(i, vectorBlockAToLevelTwo));
        vectorAlphaBlockB.push_back(getAlphaString(i, vectorBlockBToLevelTwo));
        vectorAlphaBlockC.push_back(getAlphaString(i, vectorBlockCToLevelTwo));
    }

    createBlocksVectorsSensReadingsLandH(true, 0.003, vectorBlockAToLevelTwo, vectorBlockALow);
    createBlocksVectorsSensReadingsLandH(true, 0.003, vectorBlockBToLevelTwo, vectorBlockBLow);
    createBlocksVectorsSensReadingsLandH(true, 0.003, vectorBlockCToLevelTwo, vectorBlockCLow);

    createBlocksVectorsSensReadingsLandH(false, 0.003, vectorBlockAToLevelTwo, vectorBlockAHigh);
    createBlocksVectorsSensReadingsLandH(false, 0.003, vectorBlockBToLevelTwo, vectorBlockBHigh);
    createBlocksVectorsSensReadingsLandH(false, 0.003, vectorBlockCToLevelTwo, vectorBlockCHigh);

    for (size_t i = 0; i < row; ++i)
    {
        vectorMuBlockALowerLimit.push_back(getMuLimits(i, vectorBlockALow));
        vectorMuBlockBLowerLimit.push_back(getMuLimits(i, vectorBlockBLow));
        vectorMuBlockCLowerLimit.push_back(getMuLimits(i, vectorBlockCLow));

        vectorMuBlockAUpperLimit.push_back(getMuLimits(i, vectorBlockAHigh));
        vectorMuBlockBUpperLimit.push_back(getMuLimits(i, vectorBlockBHigh));
        vectorMuBlockCUpperLimit.push_back(getMuLimits(i, vectorBlockCHigh));
    }

#ifdef _DEBUG
    qDebug() << "[LEVEL 2] Size of mu Low:" << vectorMuBlockALowerLimit.size();
    qDebug() << "[LEVEL 2] Size of mu High:" << vectorMuBlockAUpperLimit.size();
#endif

    avrg_mu_blockA = getAvrgMu(vectorMuBlockA);
    avrg_mu_blockB = getAvrgMu(vectorMuBlockB);
    avrg_mu_blockC = getAvrgMu(vectorMuBlockC);

    avrg_alpha_blockA = getAvrgAlpha(vectorAlphaBlockA);
    avrg_alpha_blockB = getAvrgAlpha(vectorAlphaBlockB);
    avrg_alpha_blockC = getAvrgAlpha(vectorAlphaBlockC);

    for (size_t i = 0; i <= row; ++i)
    {
        getMuForecast(i, 0.1, avrg_mu_blockA, vectorMuBlockA, vectorForecastMuBlockA);
        getMuForecast(i, 0.1, avrg_mu_blockB, vectorMuBlockB, vectorForecastMuBlockB);
        getMuForecast(i, 0.1, avrg_mu_blockC, vectorMuBlockC, vectorForecastMuBlockC);

        getAlphaForecast(i, 0.1, avrg_alpha_blockA, vectorAlphaBlockA, vectorForecastAlphaBlockA);
        getAlphaForecast(i, 0.1, avrg_alpha_blockB, vectorAlphaBlockB, vectorForecastAlphaBlockB);
        getAlphaForecast(i, 0.1, avrg_alpha_blockC, vectorAlphaBlockC, vectorForecastAlphaBlockC);
    }

    tableLevelTwoModel->setCurrencyVectors(vectorDateToLevelTwo,
                                           vectorMuBlockA,
                                           vectorForecastMuBlockA,
                                           vectorAlphaBlockA,
                                           vectorForecastAlphaBlockA,
                                           vectorMuBlockB,
                                           vectorForecastMuBlockB,
                                           vectorAlphaBlockB,
                                           vectorForecastAlphaBlockB,
                                           vectorMuBlockC,
                                           vectorForecastMuBlockC,
                                           vectorAlphaBlockC,
                                           vectorForecastAlphaBlockC);

    tableStabilityLevelTwoModel->setCurrencyVectors(vectorDateToLevelTwo,
                                                    vectorMuBlockALowerLimit,
                                                    vectorMuBlockA,
                                                    vectorMuBlockAUpperLimit,
                                                    vectorMuBlockBLowerLimit,
                                                    vectorMuBlockB,
                                                    vectorMuBlockBUpperLimit,
                                                    vectorMuBlockCLowerLimit,
                                                    vectorMuBlockC,
                                                    vectorMuBlockCUpperLimit);

    viewTableLevelTwoModel->selectRow(row);
}

double LevelTwoDec::getMu(unsigned int i,
                          const QVector<QVector<double> > &vectorTarget2D)
{
    double mu = 0;
    for (int j = 0; j < vectorTarget2D[0].size(); ++j)
    {
        mu += qPow(vectorTarget2D[i][j], 2);
    }
    return qSqrt(mu);
}

double LevelTwoDec::getAvrgMu(const QVector<double> &vectorTarget)
{
    double mu_avrg = 0;

    for (unsigned int i = 0; i < row; ++i)
    {
        mu_avrg += vectorTarget[i];
    }

#ifdef _DEBUG
    qDebug() << "Avgr Mu Level 2:" << mu_avrg / row;
#endif

    return mu_avrg / row;
}

double LevelTwoDec::getAvrgForecastMu(const QVector<double> &vectorTarget)
{
    double mu_avrg_forecast = 0;

    for (size_t i = 0; i < row; ++i)
    {
        mu_avrg_forecast += vectorTarget[i];
    }

#ifdef _DEBUG
    qDebug() << "Avgr Mu Forecast Level 2:" << mu_avrg_forecast / row;
#endif

    return mu_avrg_forecast / row;
}

QString LevelTwoDec::getAlphaString(unsigned int i,
                                    const QVector<QVector<double> > &vectorTarget2D)
{
    double vecMultiplic = 0;
    double muMultiplic = 0;
    if (!i)
    {
        return QString::number(0);
    }
    else
    {
        for (int j = 0; j < vectorTarget2D[0].size(); ++j)
        {
            vecMultiplic += vectorTarget2D[0][j] * vectorTarget2D[i][j];
            muMultiplic = getMu(0, vectorTarget2D) * getMu(i, vectorTarget2D);
        }
    }
    return QString::number(qAcos(vecMultiplic/muMultiplic), 'f', 6);
}

double LevelTwoDec::getAvrgAlpha(const QVector<QString> &vectorTarget)
{
    double alpha_avrg = 0;

    for (size_t i = 0; i < row; ++i)
    {
        alpha_avrg += vectorTarget[i].toDouble();
    }

#ifdef _DEBUG
    qDebug() << "Avgr Alpha Level 2:" << alpha_avrg / row;
#endif

    return alpha_avrg / row;
}

double LevelTwoDec::getAvrgForecastAlpha(const QVector<QString> &vectorTarget)
{
    double alpha_avrg_forecast = 0;

    for (size_t i = 0; i < row; ++i)
    {
        alpha_avrg_forecast += vectorTarget[i].toDouble();
    }

#ifdef _DEBUG
    qDebug() << "Avgr Alpha Forecast Level 2:" << alpha_avrg_forecast / row;
#endif

    return alpha_avrg_forecast / row;
}

void LevelTwoDec::getMuForecast(size_t i, double a, double avrg_mu,
                                const QVector<double> &vectorTarget,
                                QVector<double> &vectorOutput)
{
    double mu_forecast;

    if (i == 0)
    {
        mu_forecast = vectorTarget[0] * a + (1 - a) * avrg_mu;
    }
    else if (i > 0 && i < row)
    {
        mu_forecast = vectorTarget[i] * a + (1 - a) * vectorTarget[i - 1];
    }
    else if (i == row)
    {
        mu_forecast = getAvrgForecastMu(vectorOutput) * a + (1 - a) * vectorTarget[i - 1];
    }

    vectorOutput.push_back(mu_forecast);
}

void LevelTwoDec::getAlphaForecast(size_t i, double a, double avrg_alpha,
                                   const QVector<QString> &vectorTarget,
                                   QVector<QString> &vectorOutput)
{
    double alpha_forecast = 0;

    if (i == 0)
    {
        alpha_forecast = vectorTarget[0].toDouble() * a + (1 - a) * avrg_alpha;
    }
    else if (i > 0 && i < row)
    {
        alpha_forecast = vectorTarget[i].toDouble() * a + (1 - a) * vectorTarget[i - 1].toDouble();
    }
    else if (i == row)
    {
        alpha_forecast = getAvrgForecastAlpha(vectorOutput) * a + (1 - a) * vectorTarget[i - 1].toDouble();
    }

    vectorOutput.push_back(QString::number(alpha_forecast, 'f', 6));
}

void LevelTwoDec::createBlocksVectorsSensReadingsLandH(bool qLow,
                                                       double eps,
                                                       const QVector<QVector<double> > &vectorTarget,
                                                       QVector<QVector<double> > &vectorOutput)
{
    vectorOutput.clear();

    for (size_t i = 0; i < row; ++i)
    {
        QVector<double> tempVector;
        for (int j = 0; j < vectorTarget[0].size(); ++j)
        {
            if (qLow)
            {
                tempVector.push_back(vectorTarget[i][j] - eps);
            }
            else
            {
                tempVector.push_back(vectorTarget[i][j] + eps);
            }
        }
        vectorOutput.push_back(tempVector);
    }

#ifdef _DEBUG
    if (qLow)
    {
        qDebug() << "Mu Low Level 2[0][0]:" << vectorBlockALow[0][0];
    }
    else
    {
        qDebug() << "Mu High Level 2[0][0]:" << vectorBlockAHigh[0][0];
    }
#endif
}

double LevelTwoDec::getMuLimits(size_t i,
                                QVector<QVector<double> > &vectorTarget)
{
    double mu_lower_limit = 0;
    for (int j = 0; j < vectorTarget[0].size(); ++j)
    {
        mu_lower_limit += qPow(vectorTarget[i][j], 2);
    }
    return qSqrt(mu_lower_limit);
}

void LevelTwoDec::retranslateUi()
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

void LevelTwoDec::createTables()
{
    viewTableLevelTwoModel = new QTableView(this);

    tableLevelTwoModel = new TableLevelTwoModel(str_date,
                                                str_mu,
                                                str_alpha,
                                                str_forecast,
                                                this);

    tableStabilityLevelTwoModel = new TableStabilityLevelTwoModel(str_date,
                                                                  str_mu,
                                                                  lower_limit,
                                                                  upper_limit,
                                                                  lower_limit_short,
                                                                  upper_limit_short,
                                                                  result_str,
                                                                  stable_str,
                                                                  unstable_str,
                                                                  this);

    viewTableLevelTwoModel->setModel(tableLevelTwoModel);
}

void LevelTwoDec::readDataOfVectorsToChart()
{
    levelTwoChart->readDataOfVectors(vectorMuBlockA,
                                     vectorForecastMuBlockA,
                                     vectorAlphaBlockA,
                                     vectorForecastAlphaBlockA,
                                     vectorMuBlockB,
                                     vectorForecastMuBlockB,
                                     vectorAlphaBlockB,
                                     vectorForecastAlphaBlockB,
                                     vectorMuBlockC,
                                     vectorForecastMuBlockC,
                                     vectorAlphaBlockC,
                                     vectorForecastAlphaBlockC);
}

void LevelTwoDec::readDataOfVectorsToMuChart()
{
    levelTwoMuChart->readDataOfVectors(vectorAlphaBlockA,
                                       vectorMuBlockALowerLimit,
                                       vectorMuBlockA,
                                       vectorMuBlockAUpperLimit,
                                       vectorMuBlockBLowerLimit,
                                       vectorMuBlockB,
                                       vectorMuBlockBUpperLimit,
                                       vectorMuBlockCLowerLimit,
                                       vectorMuBlockC,
                                       vectorMuBlockCUpperLimit);
}

void LevelTwoDec::createDoubleBoxes()
{
    spbDoubleBoxAlpha = new QDoubleSpinBox(this);
    spbDoubleBoxAlpha->setRange(0.1, 1.0);
    spbDoubleBoxAlpha->setSingleStep(0.1);
    spbDoubleBoxAlpha->setWrapping(false);
    spbDoubleBoxAlpha->setToolTip(tr("Please change Coefficient \"A\""));
    connect(spbDoubleBoxAlpha, SIGNAL(valueChanged(double)), this, SLOT(changedAlpha(double)));

    spbDoubleBoxEps = new QwtCounter(this);
    spbDoubleBoxEps->setRange(0.0001, 0.02, 0.0001);
    spbDoubleBoxEps->setValue(0.003);
    spbDoubleBoxEps->setToolTip(tr("Please change Coefficient \"Eps\""));
    connect(spbDoubleBoxEps, SIGNAL(valueChanged(double)), this, SLOT(changedEps(double)));
    connect(spbDoubleBoxEps, SIGNAL(valueChanged(double)), this, SLOT(setDoubleCountToLabel(double)));
}

void LevelTwoDec::createCharts()
{
    levelTwoChart = new LevelTwoChart(this);

    readDataOfVectorsToChart();

    levelTwoMuChart = new LevelTwoMuChart(this);

    readDataOfVectorsToMuChart();
}

void LevelTwoDec::createToolBar()
{
    exportLevelTwoChart = new QAction(this);
    exportLevelTwoChart->setIcon(QIcon("://icons/chart_icons/export_chart_icon.png"));
    exportLevelTwoChart->setShortcut(QKeySequence::Print);
    exportLevelTwoChart->setText(tr("Export"));
    exportLevelTwoChart->setToolTip(tr("Export the Level One Decomposition Chart"));
    connect(exportLevelTwoChart, SIGNAL(triggered()), this, SLOT(chooseExportCharts()));

    printLevelTwoChart = new QAction(this);
    printLevelTwoChart->setIcon(QIcon("://icons/chart_icons/print_chart_icon.png"));
    printLevelTwoChart->setShortcut(QKeySequence::Save);
    printLevelTwoChart->setText(tr("Print"));
    printLevelTwoChart->setToolTip(tr("Print the Level One Decomposition Chart"));
    connect(printLevelTwoChart, SIGNAL(triggered()), this, SLOT(choosePrintCharts()));

    switchWidgetsLevelTwo = new QAction(this);
    switchWidgetsLevelTwo->setIcon(QIcon("://icons/others_icons/swap_levels_32x32.png"));
    switchWidgetsLevelTwo->setShortcut(Qt::CTRL + Qt::Key_F);
    switchWidgetsLevelTwo->setText(tr("Switch"));
    switchWidgetsLevelTwo->setToolTip(tr("Switch between charts and tables"));
    connect(switchWidgetsLevelTwo, SIGNAL(triggered()), this, SLOT(hideLevelTwoWidgets()));

    toolBar = new QToolBar(this);
    toolBar->addAction(exportLevelTwoChart);
    toolBar->addAction(printLevelTwoChart);

    toolBar->addSeparator();

    toolBar->addAction(switchWidgetsLevelTwo);

    toolBar->addSeparator();

    blocksToolTips = new QLabel(toolBar);
    blocksToolTips->setText(tr("<strong><font color='#FFCC99'>Block A</font></strong><br>"
                               "<strong><font color='#CCFFCC'>Block B</strong></font><br>"
                               "<strong><font color='#99CCFF'>Block C</strong></font><br>"));
    blocksToolTips->setContentsMargins(10, 10, 0, 0);
    blocksToolTips->setToolTip(tr("The color in the table corresponds to the tip"));

    toolBar->addWidget(blocksToolTips);

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
    labelMeterEps->setText(tr("<h3> = 3 mm</h3>"));

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

void LevelTwoDec::createWidgets()
{
    labelWarning = new QLabel(this);
    labelWarning->setText(tr("Warning! System is not stable! You need refer to a specialist or increase the coefficient \"Eps\""));
    labelWarning->setStyleSheet("QLabel { color: #FFFFFF; background-color: #FF0000 }");
    labelWarning->setContentsMargins(2, 2, 2, 2);

    horizLayoutTableLevelTwo = new QHBoxLayout;
    horizLayoutTableLevelTwo->addWidget(viewTableLevelTwoModel);
    horizLayoutTableLevelTwo->addWidget(levelTwoChart);
    horizLayoutTableLevelTwo->addWidget(levelTwoMuChart);

    vertLayoutTableLevelTwo = new QVBoxLayout(this);
    vertLayoutTableLevelTwo->addWidget(toolBar);
    vertLayoutTableLevelTwo->addLayout(horizLayoutTableLevelTwo);
    vertLayoutTableLevelTwo->addWidget(labelWarning);

    checkStability();

    levelTwoMuChart->hide();

    setLayout(vertLayoutTableLevelTwo);

    qSwitch = true;
}

void LevelTwoDec::hideLevelTwoWidgets()
{
    if (qSwitch)
    {
        viewTableLevelTwoModel->setModel(tableStabilityLevelTwoModel);

        levelTwoChart->hide();

        labelCoeffAlpha->hide();
        spbDoubleBoxAlpha->hide();


        labelCoeffEps->show();
        spbDoubleBoxEps->show();
        labelMeterEps->show();

        levelTwoMuChart->show();

        qSwitch = false;
    }
    else
    {
        viewTableLevelTwoModel->setModel(tableLevelTwoModel);
        levelTwoMuChart->hide();

        labelCoeffEps->hide();
        spbDoubleBoxEps->hide();
        labelMeterEps->hide();

        levelTwoChart->show();
        labelCoeffAlpha->show();
        spbDoubleBoxAlpha->show();

        qSwitch = true;
    }
}

void LevelTwoDec::checkStability()
{
    if (!tableStabilityLevelTwoModel->checkSystemStable())
    {
        labelWarning->show();
    }
    else
    {
        labelWarning->hide();
    }
}

void LevelTwoDec::changedAlpha(double i)
{
    vectorForecastMuBlockA.clear();
    vectorForecastMuBlockB.clear();
    vectorForecastMuBlockC.clear();
    vectorForecastAlphaBlockA.clear();
    vectorForecastAlphaBlockB.clear();
    vectorForecastAlphaBlockC.clear();

    for (size_t j = 0; j <= row; ++j)
    {
        getMuForecast(j, i, avrg_mu_blockA, vectorMuBlockA, vectorForecastMuBlockA);
        getMuForecast(j, i, avrg_mu_blockB, vectorMuBlockB, vectorForecastMuBlockB);
        getMuForecast(j, i, avrg_mu_blockC, vectorMuBlockC, vectorForecastMuBlockC);

        getAlphaForecast(j, i, avrg_alpha_blockA, vectorAlphaBlockA, vectorForecastAlphaBlockA);
        getAlphaForecast(j, i, avrg_alpha_blockB, vectorAlphaBlockB, vectorForecastAlphaBlockB);
        getAlphaForecast(j, i, avrg_alpha_blockC, vectorAlphaBlockC, vectorForecastAlphaBlockC);
    }

    tableLevelTwoModel->setCurrencyVectors(vectorDateToLevelTwo,
                                           vectorMuBlockA,
                                           vectorForecastMuBlockA,
                                           vectorAlphaBlockA,
                                           vectorForecastAlphaBlockA,
                                           vectorMuBlockB,
                                           vectorForecastMuBlockB,
                                           vectorAlphaBlockB,
                                           vectorForecastAlphaBlockB,
                                           vectorMuBlockC,
                                           vectorForecastMuBlockC,
                                           vectorAlphaBlockC,
                                           vectorForecastAlphaBlockC);

    viewTableLevelTwoModel->selectRow(row);

    // viewTableLevelOneModel->setFocus();

    readDataOfVectorsToChart();
}

void LevelTwoDec::changedEps(double eps)
{
    vectorMuBlockALowerLimit.clear();
    vectorMuBlockAUpperLimit.clear();
    vectorMuBlockBLowerLimit.clear();
    vectorMuBlockBUpperLimit.clear();
    vectorMuBlockCLowerLimit.clear();
    vectorMuBlockCUpperLimit.clear();

    createBlocksVectorsSensReadingsLandH(true, eps, vectorBlockAToLevelTwo, vectorBlockALow);
    createBlocksVectorsSensReadingsLandH(true, eps, vectorBlockBToLevelTwo, vectorBlockBLow);
    createBlocksVectorsSensReadingsLandH(true, eps, vectorBlockCToLevelTwo, vectorBlockCLow);

    createBlocksVectorsSensReadingsLandH(false, eps, vectorBlockAToLevelTwo, vectorBlockAHigh);
    createBlocksVectorsSensReadingsLandH(false, eps, vectorBlockBToLevelTwo, vectorBlockBHigh);
    createBlocksVectorsSensReadingsLandH(false, eps, vectorBlockCToLevelTwo, vectorBlockCHigh);


    for (size_t i = 0; i < row; ++i)
    {
        vectorMuBlockALowerLimit.push_back(getMuLimits(i, vectorBlockALow));
        vectorMuBlockBLowerLimit.push_back(getMuLimits(i, vectorBlockBLow));
        vectorMuBlockCLowerLimit.push_back(getMuLimits(i, vectorBlockCLow));

        vectorMuBlockAUpperLimit.push_back(getMuLimits(i, vectorBlockAHigh));
        vectorMuBlockBUpperLimit.push_back(getMuLimits(i, vectorBlockBHigh));
        vectorMuBlockCUpperLimit.push_back(getMuLimits(i, vectorBlockCHigh));
    }

    tableStabilityLevelTwoModel->setCurrencyVectors(vectorDateToLevelTwo,
                                                    vectorMuBlockALowerLimit,
                                                    vectorMuBlockA,
                                                    vectorMuBlockAUpperLimit,
                                                    vectorMuBlockBLowerLimit,
                                                    vectorMuBlockB,
                                                    vectorMuBlockBUpperLimit,
                                                    vectorMuBlockCLowerLimit,
                                                    vectorMuBlockC,
                                                    vectorMuBlockCUpperLimit);

    viewTableLevelTwoModel->selectRow(row - 1);

    tableStabilityLevelTwoModel->setCurrencyVectors(vectorDateToLevelTwo,
                                                    vectorMuBlockALowerLimit,
                                                    vectorMuBlockA,
                                                    vectorMuBlockAUpperLimit,
                                                    vectorMuBlockBLowerLimit,
                                                    vectorMuBlockB,
                                                    vectorMuBlockBUpperLimit,
                                                    vectorMuBlockCLowerLimit,
                                                    vectorMuBlockC,
                                                    vectorMuBlockCUpperLimit);

    readDataOfVectorsToMuChart();
    checkStability();
}

void LevelTwoDec::setDoubleCountToLabel(double eps)
{
    labelMeterEps->setText(QString(tr("<h3> = %1 mm</h3>")).arg(eps * 1000));
}

void LevelTwoDec::chooseExportCharts()
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
        levelTwoChart->exportLevelTwoPlotToImage();
        return;
    }
    else if (n == QMessageBox::No)
    {
        levelTwoMuChart->exportLevelTwoPlotToImage();
        return;
    }
    else if (n == QMessageBox::Close)
    {
        return;
    }
}

void LevelTwoDec::choosePrintCharts()
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
        levelTwoChart->printLevelTwoPlot();
        return;
    }
    else if (n == QMessageBox::No)
    {
        levelTwoMuChart->printLevelTwoPlot();
        return;
    }
    else if (n == QMessageBox::Close)
    {
        return;
    }
}

LevelTwoDec::~LevelTwoDec()
{
    /* Empty Destructor */
}

/********** TABLE MODEL #1 **********/
TableLevelTwoModel::TableLevelTwoModel(const QString &str_date,
                                       const QString &str_mu,
                                       const QString &str_alpha,
                                       const QString &str_forecast,
                                       QObject *parent) :
    QAbstractTableModel(parent)
{
    generator = new Generators();

    str_alpha_table = str_alpha;
    str_mu_table = str_mu;
    str_forecast_table = str_forecast;
    str_date_table = str_date;

    str_blockA = "|A";
    str_blockB = "|B";
    str_blockC = "|C";

    lightYellow.setRgb(255, 255, 153);
    lightRed.setRgb(255, 204, 153);
    lightGreen.setRgb(204, 255, 204);
    lightBlue.setRgb(153, 204, 255);
}

void TableLevelTwoModel::setCurrencyVectors(const QVector<QString> &tableVectorDate,
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
                                            const QVector<QString> &vectorForecastAlphaBlockC)
{
    row_count = vectorMuBlockA.size();

    tableVectorDateOfLevel = tableVectorDate;

    muBlockAVectorOfLevel = vectorMuBlockA;
    muBlockBVectorOfLevel = vectorMuBlockB;
    muBlockCVectorOfLevel = vectorMuBlockC;
    muBlockAForecastVectorOfLevel = vectorForecastMuBlockA;
    muBlockBForecastVectorOfLevel = vectorForecastMuBlockB;
    muBlockCForecastVectorOfLevel = vectorForecastMuBlockC;
    alphaBlockAVectorOfLevel = vectorAlphaBlockA;
    alphaBlockBVectorOfLevel = vectorAlphaBlockB;
    alphaBlockCVectorOfLevel = vectorAlphaBlockC;
    alphaBlockAForecastVectorOfLevel = vectorForecastAlphaBlockA;
    alphaBlockBForecastVectorOfLevel = vectorForecastAlphaBlockB;
    alphaBlockCForecastVectorOfLevel = vectorForecastAlphaBlockC;

    reset();
}

int TableLevelTwoModel::rowCount(const QModelIndex &) const
{
    return muBlockAForecastVectorOfLevel.size();
}

int TableLevelTwoModel::columnCount(const QModelIndex &) const
{
    return 13;
}

QVariant TableLevelTwoModel::data(const QModelIndex &index, int role) const
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
            return (index.row() < row_count) ? QVariant(muBlockAVectorOfLevel[index.row()]) : QVariant();
        case 2:
            return (index.row() < row_count) ? QVariant(alphaBlockAVectorOfLevel[index.row()]) : QVariant();
        case 3:
            return QVariant(muBlockAForecastVectorOfLevel[index.row()]);
        case 4:
            return QVariant(alphaBlockAForecastVectorOfLevel[index.row()]);
        case 5:
            return (index.row() < row_count) ? QVariant(muBlockBVectorOfLevel[index.row()]) : QVariant();
        case 6:
            return (index.row() < row_count) ? QVariant(alphaBlockBVectorOfLevel[index.row()]) : QVariant();
        case 7:
            return QVariant(muBlockBForecastVectorOfLevel[index.row()]);
        case 8:
            return QVariant(alphaBlockBForecastVectorOfLevel[index.row()]);
        case 9:
            return (index.row() < row_count) ? QVariant(muBlockCVectorOfLevel[index.row()]) : QVariant();
        case 10:
            return (index.row() < row_count) ? QVariant(alphaBlockCVectorOfLevel[index.row()]) : QVariant();
        case 11:
            return QVariant(muBlockCForecastVectorOfLevel[index.row()]);
        case 12:
            return QVariant(alphaBlockCForecastVectorOfLevel[index.row()]);
        default:
            break;
        }
    }
    else if (role == Qt::BackgroundColorRole && index.column() == 0)
    {
        return QVariant(lightYellow);
    }
    else if (role == Qt::BackgroundColorRole && (index.column() > 0 && index.column() <= 4))
    {
        return QVariant(lightRed);
    }
    else if (role == Qt::BackgroundColorRole && (index.column() > 4 && index.column() <= 8))
    {
        return QVariant(lightGreen);
    }
    else if (role == Qt::BackgroundColorRole && (index.column() > 8 && index.column() <= 12))
    {
        return QVariant(lightBlue);
    }
    return QVariant();
}

QVariant TableLevelTwoModel::headerData(int section, Qt::Orientation orientation, int role) const
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
            return str_mu_table + str_blockA;
        case 2:
            return str_alpha_table + str_blockA;
        case 3:
            return str_mu_table + " " + str_forecast_table + str_blockA;
        case 4:
            return str_alpha_table + " " + str_forecast_table + str_blockA;
        case 5:
            return str_mu_table + str_blockB;
        case 6:
            return str_alpha_table + str_blockB;
        case 7:
            return str_mu_table + " " + str_forecast_table + str_blockB;
        case 8:
            return str_alpha_table + " " + str_forecast_table + str_blockB;
        case 9:
            return str_mu_table + str_blockC;
        case 10:
            return str_alpha_table + str_blockC;
        case 11:
            return str_mu_table + " " + str_forecast_table + str_blockC;
        case 12:
            return str_alpha_table + " " + str_forecast_table + str_blockC;
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

TableLevelTwoModel::~TableLevelTwoModel()
{
    /* Empty Destructor */
}
/********** END TABLE MODEL #1 **********/

/********** TABLE MODEL #2 **********/
TableStabilityLevelTwoModel::TableStabilityLevelTwoModel(const QString &str_date,
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

    str_blockA = "|A";
    str_blockB = "|B";
    str_blockC = "|C";

    lightYellow.setRgb(255, 255, 153);
    lightRed.setRgb(255, 204, 153);
    lightGreen.setRgb(204, 255, 204);
    lightBlue.setRgb(153, 204, 255);
}

void TableStabilityLevelTwoModel::setCurrencyVectors(const QVector<QString> &tableVectorDate,
                                                     QVector<double> &vectorMuBlockALowerLimit,
                                                     QVector<double> &vectorMuBlockA,
                                                     QVector<double> &vectorMuBlockAUpperLimit,
                                                     QVector<double> &vectorMuBlockBLowerLimit,
                                                     QVector<double> &vectorMuBlockB,
                                                     QVector<double> &vectorMuBlockBUpperLimit,
                                                     QVector<double> &vectorMuBlockCLowerLimit,
                                                     QVector<double> &vectorMuBlockC,
                                                     QVector<double> &vectorMuBlockCUpperLimit)
{
    row_count = vectorMuBlockA.size();

    tableVectorDateOfLevel = tableVectorDate;

    muBlockALLVectorOfLevel = vectorMuBlockALowerLimit;
    muBlockAVectorOfLevel = vectorMuBlockA;
    muBlockAULVectorOfLevel = vectorMuBlockAUpperLimit;
    muBlockBLLVectorOfLevel = vectorMuBlockBLowerLimit;
    muBlockBVectorOfLevel = vectorMuBlockB;
    muBlockBULVectorOfLevel = vectorMuBlockBUpperLimit;
    muBlockCLLVectorOfLevel = vectorMuBlockCLowerLimit;
    muBlockCVectorOfLevel = vectorMuBlockC;
    muBlockCULVectorOfLevel = vectorMuBlockCUpperLimit;

    reset();
}

bool TableStabilityLevelTwoModel::checkSystemStable() const
{
    QVector<bool> checkSystemStableVector;

    for (size_t i = 0; i < row_count; ++i)
    {
        checkSystemStableVector.push_back(checkResult(i,
                                                      muBlockALLVectorOfLevel,
                                                      muBlockAVectorOfLevel,
                                                      muBlockAULVectorOfLevel));
        checkSystemStableVector.push_back(checkResult(i,
                                                      muBlockBLLVectorOfLevel,
                                                      muBlockBVectorOfLevel,
                                                      muBlockBULVectorOfLevel));
        checkSystemStableVector.push_back(checkResult(i,
                                                      muBlockCLLVectorOfLevel,
                                                      muBlockCVectorOfLevel,
                                                      muBlockCULVectorOfLevel));
    }

    for (size_t j = 0; j < row_count* 3; ++j)
    {
        if (!checkSystemStableVector[j])
        {
            return false;
        }
    }
    return true;
}

bool TableStabilityLevelTwoModel::checkResult(int i,
                                              const QVector<double> &vectorLower,
                                              const QVector<double> &vectorMiddle,
                                              const QVector<double> &vectorUpper) const
{
    bool check_result = (vectorUpper[i] - vectorLower[i]) >=
            (vectorMiddle[i] - vectorMiddle[0]);

    if (check_result)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int TableStabilityLevelTwoModel::rowCount(const QModelIndex &) const
{
    return muBlockAVectorOfLevel.size();
}

int TableStabilityLevelTwoModel::columnCount(const QModelIndex &) const
{
    return 19; // 19
}

QVariant TableStabilityLevelTwoModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
        case 0:
            return QVariant(tableVectorDateOfLevel[index.row()]);
        case 1:
            return QVariant(muBlockALLVectorOfLevel[index.row()]);
        case 2:
            return QVariant(muBlockAVectorOfLevel[index.row()]);
        case 3:
            return QVariant(muBlockAULVectorOfLevel[index.row()]);
        case 4:
            return QVariant(muBlockAULVectorOfLevel[index.row()] - muBlockALLVectorOfLevel[index.row()]);
        case 5:
            return (!index.row()) ?
                        QVariant(0) :
                        QVariant(muBlockAVectorOfLevel[index.row()] - muBlockAVectorOfLevel[0]);
        case 6:
            return (checkResult(index.row(),
                                muBlockALLVectorOfLevel,
                                muBlockAVectorOfLevel,
                                muBlockAULVectorOfLevel)) ?
                        QVariant(str_stable) :
                        QVariant(str_unstable);
        case 7:
            return QVariant(muBlockBLLVectorOfLevel[index.row()]);
        case 8:
            return QVariant(muBlockBVectorOfLevel[index.row()]);
        case 9:
            return QVariant(muBlockBULVectorOfLevel[index.row()]);
        case 10:
            return QVariant(muBlockBULVectorOfLevel[index.row()] - muBlockBLLVectorOfLevel[index.row()]);
        case 11:
            return (!index.row()) ?
                        QVariant(0) :
                        QVariant(muBlockBVectorOfLevel[index.row()] - muBlockBVectorOfLevel[0]);
        case 12:
            return (checkResult(index.row(),
                                muBlockBLLVectorOfLevel,
                                muBlockBVectorOfLevel,
                                muBlockBULVectorOfLevel)) ?
                        QVariant(str_stable) :
                        QVariant(str_unstable);
        case 13:
            return QVariant(muBlockCLLVectorOfLevel[index.row()]);
        case 14:
            return QVariant(muBlockCVectorOfLevel[index.row()]);
        case 15:
            return QVariant(muBlockCULVectorOfLevel[index.row()]);
        case 16:
            return QVariant(muBlockCULVectorOfLevel[index.row()] - muBlockCLLVectorOfLevel[index.row()]);
        case 17:
            return (!index.row()) ?
                        QVariant(0) :
                        QVariant(muBlockCVectorOfLevel[index.row()] - muBlockCVectorOfLevel[0]);
        case 18:
            return (checkResult(index.row(),
                                muBlockCLLVectorOfLevel,
                                muBlockCVectorOfLevel,
                                muBlockCULVectorOfLevel)) ?
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
    else if (role == Qt::BackgroundColorRole && (index.column() > 0 && index.column() < 6))
    {
        return QVariant(lightRed);
    }
    else if (role == Qt::BackgroundColorRole && (index.column() > 6 && index.column() < 12))
    {
        return QVariant(lightGreen);
    }
    else if (role == Qt::BackgroundColorRole && (index.column() > 12 && index.column() < 18))
    {
        return QVariant(lightBlue);
    }
    else if (role == Qt::BackgroundColorRole && (index.column() == 6 || index.column() == 12 || index.column() == 18))
    {
        bool qCheckBlockA = checkResult(index.row(),
                                        muBlockALLVectorOfLevel,
                                        muBlockAVectorOfLevel,
                                        muBlockAULVectorOfLevel);
        bool qCheckBlockB = checkResult(index.row(),
                                        muBlockBLLVectorOfLevel,
                                        muBlockBVectorOfLevel,
                                        muBlockBULVectorOfLevel);
        bool qCheckBlockC = checkResult(index.row(),
                                        muBlockCLLVectorOfLevel,
                                        muBlockCVectorOfLevel,
                                        muBlockCULVectorOfLevel);

        if (qCheckBlockA && index.column() == 6)
        {
            return QVariant(Qt::green);
        }
        else if (qCheckBlockB && index.column() == 12)
        {
            return QVariant(Qt::green);
        }
        else if (qCheckBlockC && index.column() == 18)
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

QVariant TableStabilityLevelTwoModel::headerData(int section, Qt::Orientation orientation, int role) const
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
            return str_mu_table + str_lower_limit + str_blockA;
        case 2:
            return str_mu_table + str_blockA;
        case 3:
            return str_mu_table + str_upper_limit + str_blockA;
        case 4:
            return str_mu_table + str_upper_limit_short + "-" + str_mu_table + str_lower_limit_short + str_blockA;
        case 5:
            return str_mu_table + "[i]-" + str_mu_table + "[0]" + str_blockA;
        case 6:
            return str_result + str_blockA;
        case 7:
            return str_mu_table + str_lower_limit + str_blockB;
        case 8:
            return str_mu_table + str_blockB;
        case 9:
            return str_mu_table + str_upper_limit + str_blockB;
        case 10:
            return str_mu_table + str_upper_limit_short + "-" + str_mu_table + str_lower_limit_short + str_blockB;
        case 11:
            return str_mu_table + "[i]-" + str_mu_table + "[0]" + str_blockB;
        case 12:
            return str_result + str_blockB;
        case 13:
            return str_mu_table + str_lower_limit + str_blockC;
        case 14:
            return str_mu_table + str_blockC;
        case 15:
            return str_mu_table + str_upper_limit + str_blockC;
        case 16:
            return str_mu_table + str_upper_limit_short + "-" + str_mu_table + str_lower_limit_short + str_blockC;
        case 17:
            return str_mu_table + "[i]-" + str_mu_table + "[0]" + str_blockC;
        case 18:
            return str_result + str_blockC;
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

TableStabilityLevelTwoModel::~TableStabilityLevelTwoModel()
{
    /* Empty Destructor */
}
/********** END TABLE MODEL #2 **********/
