#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "SchemeSensors.h"
#include "DataBaseEngine.h"
#include "LineChartWidget.h"
#include "OGLContextWidget.h"
#include "LevelsWidget.h"

#include <QTranslator>
#include <QMainWindow>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QActionGroup>
#include <QSettings>
#include <QTableView>
#include <QSplitter>

class MainWindow : public QMainWindow
{
    /* Macros for signal, slots, tr(), etc. */
    Q_OBJECT
    
    /********** Start Translators **********/
    QTranslator appTranslator;
    QTranslator qtTranslator;
    /********** End Translators **********/

    /********** Start Actions **********/
        /* Toolbar #1 */
    QAction *addRandomRowInTheDataBaseAction;
    QAction *loadTableFromHeaderAction;
    QAction *clearDataBaseAction;
        // Separator
    QAction *loadTableFromTextFileAction;
    QAction *loadTableFromDataFileAction;
    QAction *saveTableToTextFileAction;
    QAction *saveTableToDataFileAction;
    QAction *saveTableToHtmlFileAction;
        // Separator
    QAction *changeTableModelAction;

        /* Toolbar #2 */
    QAction *showChartWindowAction;
    QAction *showLevelsWindowAction;
    QAction *showOpenGLContextWindowAction;
        // Separator
    QAction *showAboutDialogWindowAction;
    QAction *showAboutQtDialogWindowAction;

        /* Other Actions */
    QActionGroup *languageActionGroup;

    QAction *setEditTablePolicy;
    QAction *setNoEditTablePolicy;

    QAction *languageAction;
    QAction *quitAppAction;
    /********** End Actions **********/

    /********** Start Menus *********/
    QMenu *fileMenu;
    QMenu *viewMenu;
    QMenu *tableMenu;
    QMenu *languageMenu;
    QMenu *helpMenu;
    /********** End Menus *********/

    /********** Start ToolBars **********/
    QToolBar *fileToolBar;
    QToolBar *viewToolBar;
    /********** End ToolBars **********/

    /********** Start Widgets **********/
    QWidget *mainWidget;
    QHBoxLayout *mainLayout;

    QGroupBox *tableGroupBox;
    QHBoxLayout *tableWidgetLayout;
    QTableView *tableWidget;

    QGroupBox *schemeGroupBox;
    QHBoxLayout *schemeWidgetLayout;
    SchemeSensors *schemeWidget;
    /********** End Widgets **********/

    /********** Start Others **********/
    QSplitter *splitter;
    QSettings *appSettings;
    QString aboutTitle;
    QString aboutBody;

    DataBaseEngine *dataBaseEngine;
    LineChartWidget *lineChartWidget;
    OGLContextWidget *oglContextWidget;
    LevelsWidget *levelsWidget;
    /********** End Others **********/

    void readGlobalSettings();
    void writeGlobalSettings();

    void createActions();
    void createAndReadDataBase();
    void createMenus();
    void createLanguageMenu();
    void createToolBars();
    void createStatusBar();
    void readSettingsFromIniFile();
    void createCentralWidget();
    void createTableGroupBox();
    void createSchemeGroupBox();
    void settingsWindow();
    void retranslateUi();
private slots:
    void switchLanguage(QAction *action);

    void setEditTablePolicySlot();
    void setNoEditTablePolicySlot();
    void switchTable();

    void showChartWindowSlot();
    void showOpenGLContextWindowSlot();
    void showLevelsWindowSlot();

    void showAboutDialog();
protected:
    void closeEvent(QCloseEvent *event);
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
