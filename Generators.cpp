#include "Generators.h"

#include <QDateTime>

Generators::Generators()
{
    /* Empty Consrtuctor */
}

double Generators::fRand(double fMin, double fMax)
{
    double f = (double)qrand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

QString Generators::dateGenerator()
{
    QDate date = QDate::currentDate();
    QString dateString = date.toString("MM.dd");
    return dateString;
}

Generators::~Generators()
{
    /* Empty Destructor */
}
