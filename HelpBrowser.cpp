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

    homeButton = new QPushButton(tr("&Home"));
    backButton = new QPushButton(tr("&Back"));
    closeButton = new QPushButton(tr("Close"));
    closeButton->setShortcut(tr("Esc"));

    buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(homeButton);
    buttonLayout->addWidget(backButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);

    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(textBrowser);

    setLayout(mainLayout);

    connect(homeButton, SIGNAL(clicked()), textBrowser, SLOT(home()));
    connect(backButton, SIGNAL(clicked()), textBrowser, SLOT(backward()));
    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(textBrowser, SIGNAL(sourceChanged(QUrl)), this, SLOT(updateWindowTitle()));

    textBrowser->setSearchPaths(QStringList() << path << ":/images");
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
