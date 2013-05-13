#ifndef OGLCONTEXTWIDGET_H
#define OGLCONTEXTWIDGET_H

#include "OGLContext.h"

#include <QWidget>
#include <QScrollArea>
#include <QGridLayout>
#include <QMenu>
#include <QMenuBar>

class OGLContextWidget : public QWidget
{
    Q_OBJECT

    QMenu *fileMenu;
    QMenuBar *glMenuBar;

    OGLContext *glWidget;
    QScrollArea *glWidgetArea;
    QGridLayout *centralLayout;

    void createGLWidgets();
    void createMenuAndActions();
    void createLayout();
    void settingsWidget();
private slots:
    void grabFrameBuffer();
public:
    OGLContextWidget(QWidget *parent = 0);
    ~OGLContextWidget();
};

#endif // OGLCONTEXTWIDGET_H
