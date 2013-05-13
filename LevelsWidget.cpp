#include "LevelsWidget.h"

#include <QAction>
#include <QStatusBar>
#include <QMenu>
#include <QMenuBar>

LevelsWidget::LevelsWidget(const QVector<QString> &vectorDate, const QVector<QVector<double> > &vectorSensorReadings2D, QWidget *parent) :
    QMainWindow(parent)
{
    levelDecOne = new LevelDecOne(vectorDate, vectorSensorReadings2D);

    createActions();

    createMenus();

    createCentralLevelsWidget();

    createStatusBar();

    settingsLevelsWindow();

    retranslateUi();

}

void LevelsWidget::createActions()
{
    quitLevelsWindowAction = new QAction(this);
    quitLevelsWindowAction->setIcon(QIcon("://icons/chart_icons/close_chart_icon.png"));
    quitLevelsWindowAction->setShortcut(Qt::Key_F10);
    connect(quitLevelsWindowAction, SIGNAL(triggered()), this, SLOT(close()));
}

void LevelsWidget::createMenus()
{
    fileLevelsMenu = new QMenu(this);
    fileLevelsMenu->addAction(quitLevelsWindowAction);

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

    menuBar()->addMenu(fileLevelsMenu);
}

void LevelsWidget::createCentralLevelsWidget()
{
    centralLevelsWidget = new QWidget(this);
    centralLayout = new QHBoxLayout(centralLevelsWidget);

    tabLevelsWidget = new QTabWidget(centralLevelsWidget);

    tabLevelsWidget->addTab(levelDecOne, tr("Level I"));
    tabLevelsWidget->addTab(new QLabel(tr("There is a second level"), tabLevelsWidget), tr("Level II"));
    tabLevelsWidget->addTab(new QLabel(tr("There is a third level"), tabLevelsWidget), tr("Level III"));

    centralLayout->addWidget(tabLevelsWidget);
    centralLevelsWidget->setLayout(centralLayout);

    setCentralWidget(centralLevelsWidget);
}

void LevelsWidget::createStatusBar()
{
    statusBar()->setStyleSheet("QStatusBar { color: #FFFFFF; background-color: #0099CC }");
}

void LevelsWidget::settingsLevelsWindow()
{
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_ShowModal);

    resize(920, 625);
    setMinimumSize(800, 480);
}

void LevelsWidget::retranslateUi()
{
    quitLevelsWindowAction->setText(tr("&Close window"));
    quitLevelsWindowAction->setStatusTip(tr("Close the levels decomposition window"));

    fileLevelsMenu->setTitle(tr("&File"));
    fileLevelsMenu->setToolTip(tr("File Menu"));

    statusBar()->showMessage(tr("Ready"));

    levelDecOne->setStatusTip(tr("First Level of Decomposition"));

    setWindowTitle(tr("Levels of decomposition"));
}

LevelsWidget::~LevelsWidget()
{
    /* Empty Destructor */
}
