#include "LevelOneDecWidget.h"

#include <QAction>
#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>
#include <qmath.h>

#ifdef _DEBUG
#include <QDebug>
#endif

LevelOneDec::LevelOneDec(const QVector<QString> &vectorDate,
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


void LevelOneDec::retranslateUi()
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

void LevelOneDec::createTables()
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

void LevelOneDec::createDoubleBoxes()
{
    spbDoubleBoxAlpha = new QDoubleSpinBox(this);
    spbDoubleBoxAlpha->setRange(0.1, 1.0);
    spbDoubleBoxAlpha->setSingleStep(0.1);
    spbDoubleBoxAlpha->setDecimals(1);
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

void LevelOneDec::createCharts()
{
    levelOneChart = new LevelOneChart(this);
    levelOneChart->readDataOfVectors(muVector,
                                     muVectorForecast,
                                     alphaVector,
                                     alphaVectorForecast);

    levelOneMuChart = new LevelOneMuChart(this);
    levelOneMuChart->readDataOfVectors(muLowerLimitVector,
                                       muVector,
                                       muUpperLimitVector,
                                       alphaVector);

}

void LevelOneDec::createToolBar()
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

    exportReportLevelOne = new QAction(this);
    exportReportLevelOne->setIcon(QIcon("://icons/save_icons/export_html_icon_32x32.png"));
    exportReportLevelOne->setShortcut(Qt::CTRL + Qt::Key_H);
    exportReportLevelOne->setText(tr("Export HTML"));
    exportReportLevelOne->setToolTip(tr("Export detailed report of Level One in the HTML page"));
    connect(exportReportLevelOne, SIGNAL(triggered()), this, SLOT(exportReportToHTML()));

    switchWidgetsLevelOne = new QAction(this);
    switchWidgetsLevelOne->setIcon(QIcon("://icons/others_icons/swap_levels_32x32.png"));
    switchWidgetsLevelOne->setShortcut(Qt::CTRL + Qt::Key_F);
    switchWidgetsLevelOne->setText(tr("Switch"));
    switchWidgetsLevelOne->setToolTip(tr("Switch between charts and tables"));
    connect(switchWidgetsLevelOne, SIGNAL(triggered()), this, SLOT(hideLevelOneWidgets()));

    toolBar = new QToolBar(this);
    toolBar->addAction(exportLevelOneChart);
    toolBar->addAction(printLevelOneChart);
    toolBar->addAction(exportReportLevelOne);

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

void LevelOneDec::createWidgets()
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

    checkStability();

    levelOneMuChart->hide();

    setLayout(vertLayoutLevelOne);

    qSwitch = true;
}

void LevelOneDec::checkStability()
{
    if (!tableStabilityLevelOneModel->checkSystemStable())
    {
        labelWarning->show();
    }
    else
    {
        labelWarning->hide();
    }
}

void LevelOneDec::setVectorsToTables()
{
    for (size_t i = 0; i < row; ++i)
    {
        muVector.push_back(getMu(i));
        alphaVector.push_back(getAlphaString(i));
    }

    createVector2DSensReadingsLow(0.003);
    createVector2DSensReadingsHigh(0.003);

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

double LevelOneDec::getMu(int i)
{
    double mu = 0;
    for (size_t j = 0; j < col; ++j)
    {
        mu += qPow(vectorSensorReadings2DToLevelOne[i][j], 2);
    }
    return qSqrt(mu);
}

double LevelOneDec::getAvrgMu()
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

QString LevelOneDec::getAlphaString(int i)
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

double LevelOneDec::getAvrgAlpha()
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

void LevelOneDec::getMuForecast(size_t i, double a)
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
        mu_forecast = getAvrgMuForecast() * a + (1 - a) * muVector[i - 1];
    }

    muVectorForecast.push_back(mu_forecast);
}

double LevelOneDec::getAvrgMuForecast()
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

void LevelOneDec::getAlphaForecast(size_t i, double a)
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
        alpha_forecast = getAvrgAlphaForecast() * a + (1 - a) * alphaVector[i - 1].toDouble();
    }

    alphaVectorForecast.push_back(QString::number(alpha_forecast, 'f', 6));
}

double LevelOneDec::getAvrgAlphaForecast()
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

void LevelOneDec::createVector2DSensReadingsLow(double eps)
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

void LevelOneDec::createVector2DSensReadingsHigh(double eps)
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

double LevelOneDec::getMuLowerLimit(size_t i)
{
    double mu_lower_limit = 0;
    for (size_t j = 0; j < col; ++j)
    {
        mu_lower_limit += qPow(vector2DSensorReadingsLow[i][j], 2);
    }
    return qSqrt(mu_lower_limit);
}

double LevelOneDec::getMuHighLimit(size_t i)
{
    double mu_high_limit = 0;
    for (size_t j = 0; j < col; ++j)
    {
        mu_high_limit += qPow(vector2DSensorReadingsHigh[i][j], 2);
    }
    return qSqrt(mu_high_limit);
}

void LevelOneDec::changedAlpha(double i)
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
    levelOneChart->readDataOfVectors(muVector,
                                     muVectorForecast,
                                     alphaVector,
                                     alphaVectorForecast);
}

void LevelOneDec::changedEps(double eps)
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

    levelOneMuChart->readDataOfVectors(muLowerLimitVector,
                                       muVector,
                                       muUpperLimitVector,
                                       alphaVector);

    checkStability();
}

void LevelOneDec::setDoubleCountToLabel(double eps)
{
    labelMeterEps->setText(QString(tr("<h3> = %1 mm</h3>")).arg(eps * 1000));
}

void LevelOneDec::chooseExportCharts()
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

void LevelOneDec::choosePrintCharts()
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

void LevelOneDec::hideLevelOneWidgets()
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

void LevelOneDec::exportReportToHTML()
{
    Generators *generator = new Generators();
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save *.html file"),
                                                    "",
                                                    tr("HTML page (*.html);;All Files (*)"));
    QFile file(fileName);
    if (!(file.open(QIODevice::WriteOnly | QIODevice::Text)))
    {
#ifdef _DEBUG
        QMessageBox::critical(0, tr("Error!"), tr("Can't write HTML file!\n"
                                                  "Please check RW permission or correct select the file.\n"
                                                  "And try again!"));
        qDebug() << "[Level 1] Error Writing HTML-file!";
#endif
        return;
    }

    /* Create TextStream for HTML */
    QTextStream out(&file);

    /* HTML Structure */
    structHTMLReportLevelOne htmlReport;

    htmlReport.header = "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n"
            "<html xmlns=\"http://www.w3.org/1999/xhtml\">\n"
            "<head>\n"
            "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n"
            "<title>Report of the first level of decomposition (LEVEL I)</title></head>\n";

    /* Taple properties and table one header */
    htmlReport.tableOne = "<center>\n<h1>Table of phase coordinates:</h1>\n<table border =\"1\" bgcolor=\"#FFFF99\" cellpadding=\"5\">\n"
            "<tr>\n<td align = \"center\"><strong><i>DATE</i></strong></td>";
    htmlReport.tableOne += "<td align = \"center\"><strong>Mu</strong></td>";
    htmlReport.tableOne += "<td align = \"center\"><strong>Alpha</strong></td>";
    htmlReport.tableOne += "<td align = \"center\"><strong>Mu Forecast</strong></td>";
    htmlReport.tableOne += "<td align = \"center\"><strong>Alpha Forecast</strong></td>";
    htmlReport.tableOne += "\n</tr>\n";
    /************************************/

    /* Taple properties and table one header */
    htmlReport.tableTwo = "\n<br/><h1>Table of available deviations:\n</h1><table border =\"1\" bgcolor=\"#FFFF99\" cellpadding=\"5\">\n"
            "<tr>\n<td align = \"center\"><strong><i>DATE</i></strong></td>";
    htmlReport.tableTwo += "<td align = \"center\"><strong>Mu[Low.Lim]</strong></td>";
    htmlReport.tableTwo += "<td align = \"center\"><strong>Mu</strong></td>";
    htmlReport.tableTwo += "<td align = \"center\"><strong>Mu[Upp.Lim]</strong></td>";
    htmlReport.tableTwo += "<td align = \"center\"><strong>Mu[Low.Lim]-Mu[Upp.Lim]</strong></td>";
    htmlReport.tableTwo += "<td align = \"center\"><strong>Mu[i]-Mu[0]</strong></td>";
    htmlReport.tableTwo += "<td align = \"center\"><strong>Result</strong></td>";
    htmlReport.tableTwo += "\n</tr>\n";
    /************************************/

    htmlReport.trOpen = "<tr>\n";
    htmlReport.trClose = "</tr>\n";

    htmlReport.tdOpen = "<td align = \"center\">";
    htmlReport.tdClose = "</td>";

    htmlReport.footer = "</center>\n"
            "</body>\n"
            "</html>";

    out << htmlReport.header;
    out << "<body bgcolor =\"#FFFFCC\">\n";

    if (!tableStabilityLevelOneModel->checkSystemStable())
    {
        out << "<font color = \"#FF0000\"><strong>Warning! System isn't stable!</strong></font><br/>";
    }

    if (muVectorForecast.size() > 3)
    {
        out << "Coefficient <strong>A</strong> is: " << (muVectorForecast[2] - muVector[1]) / (muVector[2] - muVector[1]);
        out << "<br/>Coefficient <strong>Eps</strong> is: " << vector2DSensorReadingsHigh[0][2] - vectorSensorReadings2DToLevelOne[0][2];
    }
    else
    {
        out << "Add more value in table!<br/>";
    }

    /********** TABLE ONE **********/
    out << htmlReport.tableOne;

    for(size_t i = 0; i < row; ++i)
    {
        /* Put DATE to HTML Table */
        out << htmlReport.trOpen << htmlReport.tdOpen << vectorDateToLevelOne[i] << htmlReport.tdClose;

        /* Put DATA to HTML Table */
        out << htmlReport.tdOpen << muVector[i] << htmlReport.tdClose;
        out << htmlReport.tdOpen << alphaVector[i] << htmlReport.tdClose;
        out << htmlReport.tdOpen << muVectorForecast[i] << htmlReport.tdClose;
        out << htmlReport.tdOpen << alphaVectorForecast[i] << htmlReport.tdClose;
        out << htmlReport.trClose;
    }
    out << htmlReport.trOpen;
    out << htmlReport.tdOpen << QString("Forecast (%1)").arg(generator->dateGenerator()) << htmlReport.tdClose;
    out << htmlReport.tdOpen << htmlReport.tdClose; // Empty Cell
    out << htmlReport.tdOpen << htmlReport.tdClose; // Empty Cell
    out << htmlReport.tdOpen << muVectorForecast[row] << htmlReport.tdClose;
    out << htmlReport.tdOpen << alphaVectorForecast[row] << htmlReport.tdClose;
    out << htmlReport.trClose;
    out << "</table>";
    /********** END TABLE ONE **********/

    /********** TABLE TWO **********/
    out << htmlReport.tableTwo;

    for(size_t i = 0; i < row; ++i)
    {
        bool qStable = (muUpperLimitVector[i] - muLowerLimitVector[i]) >= (qAbs(muVector[i] - muVector[0]));

        /* Put DATE to HTML Table */
        out << htmlReport.trOpen << htmlReport.tdOpen << vectorDateToLevelOne[i] << htmlReport.tdClose;

        /* Put DATA to HTML Table */
        out << htmlReport.tdOpen << muLowerLimitVector[i] << htmlReport.tdClose;
        out << htmlReport.tdOpen << muVector[i] << htmlReport.tdClose;
        out << htmlReport.tdOpen << muUpperLimitVector[i] << htmlReport.tdClose;
        out << htmlReport.tdOpen << muUpperLimitVector[i] - muLowerLimitVector[i] << htmlReport.tdClose;
        out << htmlReport.tdOpen << qAbs(muVector[i] - muVector[0]) << htmlReport.tdClose;
        (qStable) ? out << "<td align = \"center\" bgcolor=\"#00FF00\">" << "Stable" << htmlReport.tdClose
                     : out << "<td align = \"center\" bgcolor=\"#FF0000\">" << "Unstable" << htmlReport.tdClose;
        out << htmlReport.trClose;
    }
    out << "</table>";
    /********** END TABLE TWO **********/

    out << htmlReport.footer;

    file.close();

    delete generator;

    QMessageBox *pmbx = new QMessageBox(QMessageBox::Question,
                                        tr("Success!"),
                                        tr("Level One HTML-report is created successfully.\n"
                                           "Show HTML Page in your browser?"),
                                        QMessageBox::Yes | QMessageBox::No);
    int n = pmbx->exec();
    delete pmbx;

    if (n == QMessageBox::No)
    {
        return;
    }
    else if (n == QMessageBox::Yes)
    {
        /* Open HTML Table in your browser */
        QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
    }
}

LevelOneDec::~LevelOneDec()
{
    /* Empty Destructor */
}

/************** TABLE MODEL *************/

TableLevelOneModel::TableLevelOneModel(const QString &str_date,
                                       const QString &str_mu,
                                       const QString &str_alpha,
                                       const QString &str_forecast,
                                       QObject *parent)
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
                        QVariant(qAbs(muVectorOfLevel[index.row()] - muVectorOfLevel[0]));
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
            (qAbs(muVectorOfLevel[i] - muVectorOfLevel[0]));

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
