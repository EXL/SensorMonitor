#include "MainWindow.h"

#include <QApplication>
#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QDir>
#include <QStringList>
#include <QMessageBox>
#include <QCloseEvent>

#ifdef _DEBUG
#include <QDebug>
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* Set INI-file for settings */
    appSettings = new QSettings(QString("settings.ini"), QSettings::IniFormat);

    /* Install translations for App and Qt lib */
    qApp->installTranslator(&appTranslator);
    qApp->installTranslator(&qtTranslator);

    /* 0 */
    createAndReadDataBase();

    /* 1 */
    createActions();

    /* 2 */
    createMenus();

    /* 3 */
    createChangeNumSensorsDialog();

    /* 4 */
    createToolBars();

    /* 5 */
    createStatusBar();

    /* 6 */
    readSettingsFromIniFile();

    /* 7 */
    createCentralWidget();

    /* 8 */
    settingsWindow();

    /* 9 */
    readGlobalSettings();

    /* 10 */
    retranslateUi();
}


void MainWindow::readGlobalSettings()
{
    QSettings globalSettings("EXL\'s Group", "Sensor Monitor");
    restoreGeometry(globalSettings.value("geometry").toByteArray());
    restoreState(globalSettings.value("state").toByteArray());

#ifdef _DEBUG
    qDebug() << "Settings Loaded!";
#endif
}

void MainWindow::createActions()
{
    addRandomRowInTheDataBaseAction = new QAction(this);
    addRandomRowInTheDataBaseAction->setShortcut(Qt::CTRL + Qt::Key_R);
    addRandomRowInTheDataBaseAction->setIcon(QIcon("://icons/others_icons/add_row_into_table_32x32.png"));
    connect(addRandomRowInTheDataBaseAction, SIGNAL(triggered()), dataBaseEngine, SLOT(addRandomRowToSQLiteDataBase()));
    connect(addRandomRowInTheDataBaseAction, SIGNAL(triggered()), this, SLOT(switchTable()));

    loadTableFromHeaderAction = new QAction(this);
    loadTableFromHeaderAction->setShortcut(Qt::CTRL + Qt::Key_1);
    loadTableFromHeaderAction->setIcon(QIcon("://icons/others_icons/load_table_32x32.png"));
    connect(loadTableFromHeaderAction, SIGNAL(triggered()), dataBaseEngine, SLOT(loadTableFromHeader()));
    connect(loadTableFromHeaderAction, SIGNAL(triggered()), this, SLOT(switchTable()));

    clearDataBaseAction = new QAction(this);
    clearDataBaseAction->setShortcut(Qt::CTRL + Qt::Key_N);
    clearDataBaseAction->setIcon(QIcon("://icons/others_icons/clear_table_32x32.png"));
    connect(clearDataBaseAction, SIGNAL(triggered()), dataBaseEngine, SLOT(clearDataBase()));
    connect(clearDataBaseAction, SIGNAL(triggered()), this, SLOT(switchTable()));

    loadTableFromTextFileAction = new QAction(this);
    loadTableFromTextFileAction->setShortcut(Qt::CTRL + Qt::Key_T);
    loadTableFromTextFileAction->setIcon(QIcon("://icons/open_icons/open_txt_32x32.png"));
    connect(loadTableFromTextFileAction, SIGNAL(triggered()), dataBaseEngine, SLOT(loadTableFromTextFile()));
    connect(loadTableFromTextFileAction, SIGNAL(triggered()), this, SLOT(switchTable()));

    loadTableFromDataFileAction = new QAction(this);
    // loadTableFromDataFileAction->setShortcut(QKeySequence::UnknownKey);
    loadTableFromDataFileAction->setIcon(QIcon("://icons/open_icons/open_dat_32x32.png"));
    connect(loadTableFromDataFileAction, SIGNAL(triggered()), dataBaseEngine, SLOT(loadTableFromDataFile()));
    connect(loadTableFromDataFileAction, SIGNAL(triggered()), this, SLOT(switchTable()));

    saveTableToTextFileAction = new QAction(this);
    saveTableToTextFileAction->setShortcut(QKeySequence::Save);
    saveTableToTextFileAction->setIcon(QIcon("://icons/save_icons/save_txt_32x32.png"));
    connect(saveTableToTextFileAction, SIGNAL(triggered()), dataBaseEngine, SLOT(saveTableToTextFile()));

    saveTableToDataFileAction = new QAction(this);
    // saveTableToDataFileAction->setShortcut(QKeySequence::UnknownKey);
    saveTableToDataFileAction->setIcon(QIcon("://icons/save_icons/save_dat_32x32.png"));
    connect(saveTableToDataFileAction, SIGNAL(triggered()), dataBaseEngine, SLOT(saveTableToDataFile()));

    saveTableToHtmlFileAction = new QAction(this);
    // saveTableToHtmlFileAction->setShortcut(QKeySequence::UnknownKey);
    saveTableToHtmlFileAction->setIcon(QIcon("://icons/save_icons/save_html_32x32.png"));
    connect(saveTableToHtmlFileAction, SIGNAL(triggered()), dataBaseEngine, SLOT(saveTableToHtmlFile()));

    changeNumSensorsAction = new QAction(this);
    // changeNumSensorsAction->setShortcut(QKeySequence::UnknownKey);
    connect(changeNumSensorsAction, SIGNAL(triggered()), this, SLOT(showChangeNumSensorsDialog()));

    changeTableModelAction = new QAction(this);
    changeTableModelAction->setShortcut(Qt::CTRL + Qt::Key_F);
    changeTableModelAction->setIcon(QIcon("://icons/others_icons/swap_tables.png"));
    connect(changeTableModelAction, SIGNAL(triggered()), dataBaseEngine, SLOT(changeTableModel()));
    connect(changeTableModelAction, SIGNAL(triggered()), this, SLOT(switchTable()));

    setEditTablePolicy = new QAction(this);
    setEditTablePolicy->setCheckable(true);
    connect(setEditTablePolicy, SIGNAL(triggered()), this, SLOT(setEditTablePolicySlot()));

    setNoEditTablePolicy = new QAction(this);
    setNoEditTablePolicy->setCheckable(true);
    setNoEditTablePolicy->setChecked(true);
    connect(setNoEditTablePolicy, SIGNAL(triggered()), this, SLOT(setNoEditTablePolicySlot()));

    showChartWindowAction = new QAction(this);
    showChartWindowAction->setShortcut(Qt::CTRL + Qt::Key_P);
    showChartWindowAction->setIcon(QIcon("://icons/others_icons/show_plot_32x32.png"));
    connect(showChartWindowAction, SIGNAL(triggered()), this, SLOT(showChartWindowSlot()));

    showLevelsWindowAction = new QAction(this);
    showLevelsWindowAction->setShortcut(Qt::CTRL + Qt::Key_L);
    showLevelsWindowAction->setIcon(QIcon("://icons/others_icons/show_levels.png"));
    connect(showLevelsWindowAction, SIGNAL(triggered()), this, SLOT(showLevelsWindowSlot()));

    showOpenGLContextWindowAction = new QAction(this);
    // showOpenGLContextWindowAction->setShortcut(QKeySequence::UnknownKey);
    showOpenGLContextWindowAction->setIcon(QIcon("://icons/others_icons/show_open_gl_context_32x32.png"));
    connect(showOpenGLContextWindowAction, SIGNAL(triggered()), this, SLOT(showOpenGLContextWindowSlot()));

    showAboutDialogWindowAction = new QAction(this);
    // showAboutDialogWindowAction->setShortcut(QKeySequence::UnknownKey);
    showAboutDialogWindowAction->setIcon(QIcon("://icons/others_icons/about_dialog_32x32.png"));
    connect(showAboutDialogWindowAction, SIGNAL(triggered()), this, SLOT(showAboutDialog()));

    showAboutQtDialogWindowAction = new QAction(this);
    // showAboutQtDialogWindowAction->setShortcut(QKeySequence::UnknownKey);
    showAboutQtDialogWindowAction->setIcon(QIcon("://icons/others_icons/about_Qt_32x32.png"));
    connect(showAboutQtDialogWindowAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    quitAppAction = new QAction(this);
    quitAppAction->setShortcut(Qt::Key_F10);
    connect(quitAppAction, SIGNAL(triggered()), this, SLOT(close()));
}

void MainWindow::createAndReadDataBase()
{
    dataBaseEngine = new DataBaseEngine(this);
    dataBaseEngine->connectToSQLiteDataBase();
    dataBaseEngine->createTableInSQLiteDataBase();
    dataBaseEngine->formTableInSQLiteDataBase();
}

void MainWindow::createMenus()
{
    fileMenu = new QMenu(this);
    fileMenu->addAction(addRandomRowInTheDataBaseAction);
    fileMenu->addAction(loadTableFromHeaderAction);
    fileMenu->addAction(clearDataBaseAction);

    fileMenu->addSeparator();

    fileMenu->addAction(loadTableFromTextFileAction);
    fileMenu->addAction(loadTableFromDataFileAction);

    fileMenu->addSeparator();

    fileMenu->addAction(saveTableToTextFileAction);
    fileMenu->addAction(saveTableToDataFileAction);
    fileMenu->addAction(saveTableToHtmlFileAction);

    fileMenu->addSeparator();

    fileMenu->addAction(quitAppAction);

    viewMenu = new QMenu(this);
    viewMenu->addAction(showChartWindowAction);
    viewMenu->addAction(showLevelsWindowAction);
    viewMenu->addAction(showOpenGLContextWindowAction);

    tableMenu = new QMenu(this);
    tableMenu->addAction(setEditTablePolicy);
    tableMenu->addAction(setNoEditTablePolicy);

    tableMenu->addSeparator();

    tableMenu->addAction(changeNumSensorsAction);
    tableMenu->addAction(changeTableModelAction);

    createLanguageMenu();

    helpMenu = new QMenu(this);
    helpMenu->addAction(showAboutDialogWindowAction);
    helpMenu->addAction(showAboutQtDialogWindowAction);

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

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(viewMenu);
    menuBar()->addMenu(tableMenu);
    menuBar()->addMenu(languageMenu);
    menuBar()->addMenu(helpMenu);
}

void MainWindow::createLanguageMenu()
{
    languageMenu = new QMenu(this);

    languageActionGroup = new QActionGroup(this);
    connect(languageActionGroup, SIGNAL(triggered(QAction *)), this, SLOT(switchLanguage(QAction *)));

    QDir qmDir("://translations");
    QStringList fileNames = qmDir.entryList(QStringList("sensormonitor_*.qm"));

    for (int i = 0; i < fileNames.size(); ++i)
    {
        QString locale = fileNames[i];
        locale.remove(0, locale.indexOf('_') + 1);
        locale.chop(3);

        QTranslator translator;
        translator.load(fileNames[i], qmDir.absolutePath());

        QString language = (locale == "en") ? "English" : "Russian";

        languageAction = new QAction(tr("&%1").arg(language), this);
        languageAction->setCheckable(true);
        languageAction->setData(locale);

        languageMenu->addAction(languageAction);
        languageActionGroup->addAction(languageAction);

        if (language == "English")
            languageAction->setChecked(true);
    }
}

void MainWindow::createChangeNumSensorsDialog()
{

}

void MainWindow::readSettingsFromIniFile()
{
    QString locale = appSettings->value("Language").toString();

    QDir qmDir("://translations");
    QString qmPath = qmDir.absolutePath();

    appTranslator.load("sensormonitor_" + locale, qmPath);
    qtTranslator.load("qt_" + locale, qmPath);

    if (locale == "ru")
        languageAction->setChecked(true);
}

void MainWindow::createToolBars()
{
    fileToolBar = new QToolBar(this);
    fileToolBar->setObjectName("FileToolBar");
    fileToolBar->addAction(addRandomRowInTheDataBaseAction);
    fileToolBar->addAction(loadTableFromHeaderAction);
    fileToolBar->addAction(clearDataBaseAction);

    fileToolBar->addSeparator();

    fileToolBar->addAction(loadTableFromTextFileAction);
    fileToolBar->addAction(loadTableFromDataFileAction);

    fileToolBar->addSeparator();

    fileToolBar->addAction(saveTableToTextFileAction);
    fileToolBar->addAction(saveTableToDataFileAction);
    fileToolBar->addAction(saveTableToHtmlFileAction);

    fileToolBar->addSeparator();

    fileToolBar->addAction(changeTableModelAction);

    //    fileToolBar->addSeparator();

    //    fileToolBar->addWidget(spinBoxForDialog);

    fileToolBar->setStyleSheet("QToolBar {background-color: #D9F1FF}");

    viewToolBar = new QToolBar(this);
    viewToolBar->setObjectName("View ToolBar");
    viewToolBar->addAction(showChartWindowAction);
    viewToolBar->addAction(showLevelsWindowAction);
    viewToolBar->addAction(showOpenGLContextWindowAction);

    viewToolBar->addSeparator();

    viewToolBar->addAction(showAboutDialogWindowAction);
    viewToolBar->addAction(showAboutQtDialogWindowAction);

    viewToolBar->setStyleSheet("QToolBar {background-color: #D9F1FF}");

    addToolBar(fileToolBar);
    addToolBar(viewToolBar);
}

void MainWindow::createStatusBar()
{
    statusBar()->setStyleSheet("QStatusBar { color: #FFFFFF; background-color: #0099CC }");
}

void MainWindow::createCentralWidget()
{
    mainWidget = new QWidget(this);
    mainLayout = new QHBoxLayout(mainWidget);
    splitter = new QSplitter(mainWidget);
    splitter->setObjectName("splitter");

    createTableGroupBox();
    createSchemeGroupBox();

    splitter->addWidget(tableGroupBox);
    splitter->addWidget(schemeGroupBox);

    mainLayout->addWidget(splitter);

    mainWidget->setLayout(mainLayout);

    setCentralWidget(mainWidget);
}

void MainWindow::createTableGroupBox()
{
    tableGroupBox = new QGroupBox(mainWidget);
    tableWidgetLayout = new QHBoxLayout(tableGroupBox);

    tableWidgetLayout->addWidget(dataBaseEngine->getTableViewWidget());
}

void MainWindow::createSchemeGroupBox()
{
    schemeGroupBox = new QGroupBox(mainWidget);
    schemeGroupBox->setMaximumWidth(350);
    schemeWidgetLayout = new QHBoxLayout(schemeGroupBox);

    schemeWidget = new SchemeSensors(this);
    schemeWidgetLayout->addWidget(schemeWidget);
}

void MainWindow::settingsWindow()
{
    setWindowIcon(QIcon("://icons/sensormonitor_icon.png"));
    setMinimumSize(800, 600);
    setWindowState(Qt::WindowMaximized);
}

void MainWindow::retranslateUi()
{
    /********** Start About Dialog **********/
    aboutTitle = tr("About SensorMonitor");
    aboutBody = tr("<p><b>Version 0.57</b></p>"
                   "<p>Application and source code available under license GPL v.3.0:</p>"
                   "<p><a href='http://www.gnu.org/licenses/gpl.html'>GNU GENERAL PUBLIC LICENSE</a></p>"
                   "<p><a href='https://github.com/EXLMOTODEV/SensorMonitor'>Source Code on GitHub</a></p>"
                   "<p>Sensor Monitor is designed by Vitaly Shuvaev and Alexander Skorlupin, is students of group BI21-A.</p>"
                   "<p>Email: <a href='mailto:exl@bk.ru'>exl@bk.ru</a></p>"
                   "<p><center><a href='http://ssga.ru'><img src='://gfx/logo.png'/></a></center></p>"
                   "<p>(c) SSGA, 2013</p>"
                   "<p>Application was developmented used the following technologies:</p>"
                   "<p><table>"
                   "<tr><td align='center'><a href='http://www.w3.org/TR/html-markup/'><img src='://gfx/html.png'/></a></td><td align='center'><br>HTML and CSS is the main web markup language.</td></tr>"
                   "<tr><td align='center'><a href='http://www.sqlite.org/'><img src='://gfx/sqlite.png'/></a></td><td align='center'><br>SQLite is an embedded SQL database engine.</td></tr>"
                   "<tr><td align='center'><a href='http://qwt.sourceforge.net/'><img src='://gfx/qwt.png'/></a></td><td align='center'><br>Qwt is Qt Widgets for Technical Applications.</td></tr>"
                   "<tr><td align='center'><a href='http://www.opengl.org/'><img src='://gfx/opengl.png'/></a></td><td align='center'><br>OpenGL is a library (API) for rendering 2D/3D graphics.</td></tr>"
                   "</table></p>");
    /********** End About Dialog **********/

    /********** Start Actions **********/
    addRandomRowInTheDataBaseAction->setText(tr("&Add a random row"));
    addRandomRowInTheDataBaseAction->setStatusTip(tr("Add a random row in the SQLite DataBase"));

    loadTableFromHeaderAction->setText(tr("&Load Table"));
    loadTableFromHeaderAction->setStatusTip(tr("Load table with the sensor readings"));

    clearDataBaseAction->setText(tr("&Clear DataBase"));
    clearDataBaseAction->setStatusTip(tr("Clear the SQLite DataBase"));

    loadTableFromTextFileAction->setText(tr("&Load Table from \"*.txt\""));
    loadTableFromTextFileAction->setStatusTip(tr("Load Table from general TextFile"));

    loadTableFromDataFileAction->setText(tr("&Load Table from \"*.dat\""));
    loadTableFromDataFileAction->setStatusTip(tr("Load Table from special DataFile"));

    saveTableToTextFileAction->setText(tr("&Save Table to \"*.txt\""));
    saveTableToTextFileAction->setStatusTip(tr("Save Table to the general TextFile"));

    saveTableToDataFileAction->setText(tr("&Save Table to \"*.dat\""));
    saveTableToDataFileAction->setStatusTip(tr("Save Table to special DataFile"));

    saveTableToHtmlFileAction->setText(tr("&Export DataBase to HTLM page"));
    saveTableToHtmlFileAction->setStatusTip(tr("Export DataBase table to general HTLM page with table"));

    changeNumSensorsAction->setText(tr("&Change the number of sensors"));
    changeNumSensorsAction->setStatusTip(tr("Change the number of sensors in SQLite DataBase"));

    changeTableModelAction->setText(tr("&Swap tables SQLite <-> Memory"));
    changeTableModelAction->setStatusTip(tr("Switch SQLite DataBase Table on a Table in Memory and back"));

    setNoEditTablePolicy->setText(tr("&Disable editing table"));
    setNoEditTablePolicy->setStatusTip(tr("Disable editing SQLite DataBase table"));

    setEditTablePolicy->setText(tr("&Enable editing table"));
    setEditTablePolicy->setStatusTip(tr("Enable editing SQLite DataBase table"));

    showChartWindowAction->setText(tr("&Show Chart window"));
    showChartWindowAction->setStatusTip(tr("Show linear schedule of Sensor Monitor Table"));

    showLevelsWindowAction->setText(tr("&Show Levels of Decomposition"));
    showLevelsWindowAction->setStatusTip(tr("Show levels of decomposition windows of current DataBase"));

    showOpenGLContextWindowAction->setText(tr("&Show 3D OpenGL window"));
    showOpenGLContextWindowAction->setStatusTip(tr("Show OpenGL context window with 3D Plot"));

    showAboutDialogWindowAction->setText(tr("&About SensorMonitor"));
    showAboutDialogWindowAction->setStatusTip(tr("Show About SensorMonitor dialog"));

    showAboutQtDialogWindowAction->setText(tr("&About Qt"));
    showAboutQtDialogWindowAction->setStatusTip(tr("Show About Qt Library dialog"));

    quitAppAction->setText(tr("&Quit"));
    quitAppAction->setStatusTip(tr("Quit the program"));
    /********** End Actions ***********/

    /********** Start Menus **********/
    fileMenu->setTitle(tr("&File"));
    fileMenu->setToolTip(tr("File Menu"));

    viewMenu->setTitle(tr("&View"));
    viewMenu->setToolTip(tr("View Menu"));

    tableMenu->setTitle(tr("&Table"));
    tableMenu->setToolTip(tr("Table menu"));

    languageMenu->setTitle(tr("&Language"));
    languageMenu->setToolTip(tr("Language Menu"));

    helpMenu->setTitle(tr("&Help"));
    helpMenu->setToolTip(tr("Help Menu"));
    /********** End Menus **********/

    /********** Start ToolBars **********/
    fileToolBar->setWindowTitle(tr("File ToolBar"));
    viewToolBar->setWindowTitle(tr("View ToolBar"));
    /********** End ToolBars **********/

    /********** Start StatusBar **********/
    statusBar()->showMessage(tr("Ready"));
    /********** End StatusBar **********/

    /********** Start Central Widget **********/
    tableGroupBox->setTitle(tr("SQLite DataBase"));
    tableGroupBox->setStatusTip(tr("Table Monitoring Sensors of SQLite DataBase"));
    tableGroupBox->setToolTip(tr("Monitoring Table"));

    // splitter->setToolTip(tr("Change Size"));

    schemeGroupBox->setTitle(tr("Sensors Scheme"));
    schemeGroupBox->setStatusTip(tr("Arrangement of sensors on the building. Click to display sensors"));
    // schemeGroupBox->setToolTip(tr("Sensors Scheme"));
    /********** End Central Widget **********/

    /********** Start Other Widgets **********/
    dataBaseEngine->retranslateUi();
    schemeWidget->retranslateUi();
    /********** End Others Widgets **********/

    setWindowTitle(tr("Sensor Monitor"));
}

/* Slots */

void MainWindow::switchLanguage(QAction *action)
{
    QString locale = action->data().toString();

    appSettings->setValue("Language", locale);

    QDir qmDir("://translations");
    QString qmPath = qmDir.absolutePath();

    appTranslator.load("sensormonitor_" + locale, qmPath);
    qtTranslator.load("qt_" + locale, qmPath);

    retranslateUi();
}

void MainWindow::setEditTablePolicySlot()
{
    setEditTablePolicy->setChecked(true);
    setNoEditTablePolicy->setChecked(false);
    dataBaseEngine->setEditTablePolicy();
}

void MainWindow::setNoEditTablePolicySlot()
{
    setNoEditTablePolicy->setChecked(true);
    setEditTablePolicy->setChecked(false);
    dataBaseEngine->setNoEditTablePolicy();
}

void MainWindow::switchTable()
{
    if (dataBaseEngine->getSwitchTable())
    {
        tableGroupBox->setTitle(tr("SQLite DataBase"));
        tableGroupBox->setStatusTip(tr("Table Monitoring Sensors of SQLite DataBase"));
    }
    else
    {
        tableGroupBox->setTitle(tr("Table in Memory"));
        tableGroupBox->setStatusTip(tr("Table Monitoring Sensors of Memory RAM"));
    }
}

void MainWindow::showChartWindowSlot()
{
    if (dataBaseEngine->getSensorsDateVector().size() != 0)
    {
        lineChartWidget = new LineChartWidget(dataBaseEngine->getSensorsDateVector(), dataBaseEngine->getSensorsReadingsVector2D());
        lineChartWidget->show();
    }
    else
    {
        dataBaseEngine->emptyDataBaseCriticalError();
    }
}

void MainWindow::showOpenGLContextWindowSlot()
{
    oglContextWidget = new OGLContextWidget;
    oglContextWidget->show();
}

void MainWindow::showLevelsWindowSlot()
{
    if (dataBaseEngine->getSensorsDateVector().size() != 0)
    {
        levelsWidget = new LevelsWidget(dataBaseEngine->getSensorsDateVector(), dataBaseEngine->getSensorsReadingsVector2D());
        levelsWidget->show();
    }
    else
    {
        dataBaseEngine->emptyDataBaseCriticalError();
    }
}

void MainWindow::showAboutDialog()
{
    QMessageBox::about(this, aboutTitle, aboutBody);
}

void MainWindow::showChangeNumSensorsDialog()
{
    NumSensorsDialog dialog(dataBaseEngine->getColumnsCount());
    dialog.retranslateUi();
    dialog.exec();

    if (dialog.getClicked())
    {
        dataBaseEngine->setColumnsGlobal(dialog.getCountColumn());
    }
}

void MainWindow::writeGlobalSettings()
{
    QSettings globalSettings("EXL\'s Group", "Sensor Monitor");
    globalSettings.setValue("geometry", saveGeometry());
    globalSettings.setValue("state", saveState());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    writeGlobalSettings();

#ifdef _DEBUG
    qDebug() << "Settings Saved!";
#endif

    event->accept();
}

MainWindow::~MainWindow()
{
    /* Empty destructor */
}
