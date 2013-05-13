#ifndef SCHEMESENSORS_H
#define SCHEMESENSORS_H

#include <QWidget>

class SchemeSensors : public QWidget
{
    Q_OBJECT

    bool qDrawSensors;
    QString blockA;
    QString blockB;
    QString blockC;

    int area(int ax, int ay, int bx, int by, int cx, int cy);
    void drawRotateText(QPainter *painter,
                        float degrees,
                        int x, int y,
                        const QString &text);
    void drawSensors();
    void settingsWidget();
protected:
    void paintEvent(QPaintEvent *);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
public:
    SchemeSensors(QWidget *parent = 0);
    void retranslateUi();
    ~SchemeSensors();
};

#endif // SCHEMESENSORS_H
