#ifndef HELPBROWSER_H
#define HELPBROWSER_H

#include <QWidget>
#include <QTextBrowser>
#include <QToolBar>
#include <QAction>
#include <QHBoxLayout>
#include <QVBoxLayout>


class HelpBrowser : public QWidget
{
    Q_OBJECT

    QTextBrowser *textBrowser;
    QAction *homeAction;
    QAction *backAction;
    QAction *closeAction;

    QToolBar *toolBar;

    QHBoxLayout *buttonLayout;
    QVBoxLayout *mainLayout;

private slots:
    void updateWindowTitle();
public:
    HelpBrowser(const QString &path,
                const QString &page,
                QWidget *parent = 0);
    static void showPage(const QString &page);
};

#endif // HELPBROWSER_H
