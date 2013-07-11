#include "HelpBrowser.h"

#include <QDir>

HelpBrowser::HelpBrowser(const QString &path,
                         const QString &page,
                         QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_GroupLeader);

    textBrowser = new QTextBrowser;

    homeAction = new QAction(this);
    homeAction->setText(tr("&Home"));
    homeAction->setToolTip(tr("Go back to home"));
    homeAction->setIcon(QIcon("://icons/others_icons/help_home_32x32.png"));

    backAction = new QAction(this);
    backAction->setText(tr("&Back"));
    backAction->setToolTip(tr("Back"));
    backAction->setIcon(QIcon("://icons/others_icons/help_back_32x32.png"));

    closeAction = new QAction(this);
    closeAction->setText(tr("&Close"));
    closeAction->setToolTip(tr("Close"));
    closeAction->setIcon(QIcon("://icons/others_icons/help_close_32x32.png"));

    toolBar = new QToolBar;
    toolBar->addAction(homeAction);
    toolBar->addAction(backAction);

    toolBar->addSeparator();

    toolBar->addAction(closeAction);
    toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    mainLayout = new QVBoxLayout;
    mainLayout->addWidget(toolBar);
    mainLayout->addWidget(textBrowser);

    setLayout(mainLayout);

    connect(homeAction, SIGNAL(triggered()), textBrowser, SLOT(home()));
    connect(backAction, SIGNAL(triggered()), textBrowser, SLOT(backward()));
    connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(textBrowser, SIGNAL(sourceChanged(QUrl)), this, SLOT(updateWindowTitle()));

    textBrowser->setSearchPaths(QStringList() << path << "images");
    textBrowser->setSource(page);
}

void HelpBrowser::showPage(const QString &page)
{
    QString path = QDir("doc").absolutePath();
    HelpBrowser *showTextBrowser = new HelpBrowser(path, page);
    showTextBrowser->resize(500, 400);
    showTextBrowser->show();
}

void HelpBrowser::updateWindowTitle()
{
    setWindowTitle(tr("Help: %1").arg(textBrowser->documentTitle()));
}
