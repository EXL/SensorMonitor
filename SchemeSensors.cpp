#include "SchemeSensors.h"

#ifdef _DEBUG
#include <QDebug>
#endif

#include <QPainter>
#include <QToolTip>
#include <QMouseEvent>
#include <QPixmap>
#include <qmath.h>

SchemeSensors::SchemeSensors(QWidget *parent)
    : QWidget(parent)
{
    retranslateUi();

    qDrawSensors = false;

    settingsWidget();
}

void SchemeSensors::paintEvent(QPaintEvent *)
{
    QPainter schemePainter(this);

    schemePainter.setRenderHint(QPainter::Antialiasing, true);
    schemePainter.setBrush(QBrush(Qt::gray, Qt::DiagCrossPattern));
    schemePainter.setPen(QPen(QColor(0, 153, 204), 2));

    /* Draw Rects */
    schemePainter.drawRect(QRect(30, 5, width() - 60, height() / 8));
    schemePainter.drawRect(QRect(100, 5 + height() / 8, width() - 200, (height() / 4) + (height() / 8)));

    /* Draw Polygon */
    QPolygonF polygon;
    polygon << QPointF(10, (5 + height() / 8) + (height() / 4) + (height() / 8));
    polygon << QPointF(width() - 20, (5 + height() / 8) + (height() / 4) + (height() / 8));
    polygon << QPointF(width() - 20, 105 + (height() / 2));
    polygon << QPointF(width() / 2, 205 + (height() / 2));
    polygon << QPointF(10, 105 + (height() / 2));
    polygon << QPointF(10, (5 + height() / 8) + (height() / 4) + (height() / 8));
    schemePainter.drawPolygon(polygon);

    /* Draw Text */
    QPainter textPainter(this);
    drawRotateText(&textPainter, 270, 25, (height() / 8) - 5, blockA);
    drawRotateText(&textPainter, 270, 95, ((height() / 4) + (height() / 8)) - 5, blockB);
    drawRotateText(&textPainter, 90, width() - 15, (30 + height() / 8) + (height() / 4) + (height() / 8), blockC);

    if (qDrawSensors)
    {
        drawSensors();
    }
}

void SchemeSensors::drawRotateText(QPainter *painter,
                                   float degrees,
                                   int x, int y,
                                   const QString &text)
{
    painter->save();
    painter->translate(x, y);
    painter->rotate(degrees);
    painter->drawText(0, 0, text);
    painter->restore();
}

void SchemeSensors::drawSensors()
{
    /* Draw Sensors */
    QPixmap sensors[16];

    for (size_t i = 0; i < 16; ++i)
    {
        sensors[i].convertFromImage(QImage(QString("://gfx/Sensors/Sensor-%1.png").arg(i+1)));
    }

    QPainter sensorPainter(this);

    sensorPainter.drawPixmap(25, 10, sensors[0]);
    sensorPainter.drawPixmap(width() - 35, 10, sensors[8]);

    sensorPainter.drawPixmap(55, 0, sensors[1]);
    sensorPainter.drawPixmap(55, height() / 8, sensors[9]);

    sensorPainter.drawPixmap(width() - 70, 0, sensors[2]);
    sensorPainter.drawPixmap(width() - 70, height() / 8, sensors[10]);

    sensorPainter.drawPixmap(95, 10 + height() / 8, sensors[3]);
    sensorPainter.drawPixmap(width() - 105, 10 + height() / 8, sensors[11]);

    sensorPainter.drawPixmap(95, 50 + height() / 8, sensors[4]);
    sensorPainter.drawPixmap(width() - 105, 50 + height() / 8, sensors[12]);

    sensorPainter.drawPixmap(95, ((height() / 8) + (height() / 4) + (height() / 8)) - 15, sensors[5]);
    sensorPainter.drawPixmap(width() - 105, ((height() / 8) + (height() / 4) + (height() / 8)) - 15, sensors[13]);

    sensorPainter.drawPixmap(50, ((height() / 8) + (height() / 4) + (height() / 8)), sensors[6]);
    sensorPainter.drawPixmap(50, (130 + height() / 8) + (height() / 4) + (height() / 8), sensors[14]);

    sensorPainter.drawPixmap(width() - 65, (height() / 8) + (height() / 4) + (height() / 8), sensors[7]);
    sensorPainter.drawPixmap(width() - 65, (130 + height() / 8) + (height() / 4) + (height() / 8), sensors[15]);
}

void SchemeSensors::settingsWidget()
{
    setMinimumWidth(250);
    setMouseTracking(true);
}

void SchemeSensors::retranslateUi()
{
    blockA = tr("Block \"A\"");
    blockB = tr("Block \"B\"");
    blockC = tr("Block \"C\"");
}

void SchemeSensors::mouseMoveEvent(QMouseEvent *event)
{
    bool qPolALimits =
            event->x() >= 30 &&
            event->x() <= width() - 60 &&
            event->y() >= 5 &&
            event->y() <= height() / 8;

    bool qPolBLimits =
            event->x() >= 80 &&
            event->x() <= width() - 160 &&
            event->y() >= 5 + height() / 8 &&
            event->y() <= (height() / 4) + (height() / 8);

    bool qPolCLimits =
            event->x() >= 10 &&
            event->x() <= width() - 20 &&
            event->y() >= (5 + height() / 8) + (height() / 4) + (height() / 8) &&
            event->y() <= 105 + (height() / 2);

    bool qPolCLimitsTriangle =
            area(width() - 20, 105 + (height() / 2), width() / 2, 205 + (height() / 2), 10, 105 + (height() / 2)) ==
            area(width() - 20, 105 + (height() / 2), width() / 2, 205 + (height() / 2), event->x(), event->y()) +
            area(width() - 20, 105 + (height() / 2), event->x(), event->y(), 10, 105 + (height() / 2)) +
            area(width() / 2, 205 + (height() / 2), event->x(), event->y(), 10, 105 + (height() / 2));


    if (qPolALimits)
    {
        QToolTip::showText((QPoint(event->globalX(), event->globalY())),
                           blockA, this);
    }
    else if (qPolBLimits)
    {
        QToolTip::showText((QPoint(event->globalX(), event->globalY())),
                           blockB, this);
    }
    else if (qPolCLimits || qPolCLimitsTriangle)
    {
        QToolTip::showText((QPoint(event->globalX(), event->globalY())),
                           blockC, this);
    }
}

void SchemeSensors::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        qDrawSensors = (qDrawSensors) ? false : true;
        repaint();
    }
}

int SchemeSensors::area(int ax, int ay, int bx, int by, int cx, int cy)
{
    return qAbs((ax - cx) * (by - cy) + (bx - cx) * (cy - ay));
}

SchemeSensors::~SchemeSensors()
{
    /* Empty Destructor */
}
