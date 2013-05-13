#ifndef LEVELSWIDGET_H
#define LEVELSWIDGET_H

#include "LevelDecOneWidget.h"

#include <QMainWindow>

class LevelsWidget : public QMainWindow
{
    Q_OBJECT

    QAction *quitLevelsWindowAction;

    QMenu *fileLevelsMenu;

    QTabWidget *tabLevelsWidget;
    QHBoxLayout *centralLayout;
    QWidget *centralLevelsWidget;

    LevelDecOne *levelDecOne;

    void createActions();
    void createMenus();
    void createCentralLevelsWidget();
    void createStatusBar();
    void settingsLevelsWindow();
    void retranslateUi();
public:
    LevelsWidget(const QVector<QString> &vectorDate, const QVector<QVector<double> > &vectorSensorReadings2D, QWidget *parent = 0);
    ~LevelsWidget();
    
};

#endif // LEVELSWIDGET_H
