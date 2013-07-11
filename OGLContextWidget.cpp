#include "OGLContextWidget.h"

#include <QFileDialog>
#include <QMessageBox>

#ifdef _DEBUG
#include <QDebug>
#endif

OGLContextWidget::OGLContextWidget(QWidget *parent) :
    QWidget(parent)
{
    createGLWidgets();

    createMenuAndActions();

    createLayout();

    settingsWidget();
}

void OGLContextWidget::createGLWidgets()
{
    glWidget = new OGLContext;
    glWidgetArea = new QScrollArea;
    glWidgetArea->setWidget(glWidget);
    glWidgetArea->setWidgetResizable(true);
    glWidgetArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    glWidgetArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    glWidgetArea->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    /* For KeyEvent! */
    glWidget->setFocusPolicy(Qt::StrongFocus);
}

void OGLContextWidget::createMenuAndActions()
{
    fileMenu = new QMenu(this);
    fileMenu->setTitle(tr("&File"));

    fileMenu->addAction(QIcon("://icons/chart_icons/export_chart_icon.png"), tr("&Grab to \"*.png\""), this, SLOT(grabFrameBuffer()), Qt::Key_F6);

    fileMenu->addSeparator();

    fileMenu->addAction(QIcon("://icons/chart_icons/close_chart_icon.png"), tr("&Close window"), this, SLOT(close()), Qt::Key_F10);

    glMenuBar = new QMenuBar(this);
    glMenuBar->setStyleSheet("QMenuBar {"
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

    glMenuBar->addMenu(fileMenu);
}

void OGLContextWidget::createLayout()
{
    centralLayout = new QGridLayout(this);
    centralLayout->setMenuBar(glMenuBar);
    centralLayout->addWidget(glWidgetArea, 0, 0);
    setLayout(centralLayout);
}

void OGLContextWidget::settingsWidget()
{
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_ShowModal);

    move(128, 100);
    setMinimumSize(320, 320);
    resize(520, 520);
    setWindowTitle(tr("3D OpenGL Context"));
}

void OGLContextWidget::grabFrameBuffer()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save *.png file"), "",
                                                    tr("Image File (*.png);;All Files (*)"));
#ifdef _DEBUG
    qDebug() << "Grabbing...";
#endif
    QImage image = glWidget->grabFrameBuffer();
    QPixmap::fromImage(image);
    if (image.save(fileName, "PNG"))
    {
#ifdef _DEBUG
        qDebug() << "Grabbed!";
#endif
    }
    else
    {
        QMessageBox::critical(0, tr("Error!"), QString(tr("Can't write PNG image!\n"
                                                          "Please check RW permission or correct select the file.\n"
                                                          "And try again!")));
        return;
    }
}

OGLContextWidget::~OGLContextWidget()
{
    /* Empty Destructor */
}

