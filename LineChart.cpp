#include "LineChart.h"

#include <QFileDialog>
#include <QImageWriter>
#include <QPrintDialog>
#include <QPrinter>

#ifdef _DEBUG
#include <QDebug>
#endif

#include <qwt_plot_panner.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_magnifier.h>
#include <qwt_legend.h>
#include <qwt_legend_item.h>
#include <qwt_symbol.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_renderer.h>

const int column = 16;

LineChart::LineChart(const QVector<QString> &vectorDate,
                     const QVector<QVector<double> > &vectorSensorReadings2D,
                     QWidget *parent)
    : QwtPlot(parent)
{
    setAutoReplot(false);

    vectorDateToChart = vectorDate;
    vectorSensorReadings2DToChart = vectorSensorReadings2D;

    /* Set row an column */
    row = vectorSensorReadings2DToChart.size();
    col = vectorSensorReadings2DToChart[0].size();

    /* Panning with the left mouse button */
    (void)new QwtPlotPanner(canvas());

    /* Zoom in/out with the wheel */
    (void)new QwtPlotMagnifier(canvas());

    /* Picker  */
    plotPicker = new QwtPlotPicker(QwtPlot::xBottom,
                                   QwtPlot::yLeft,
                                   QwtPlotPicker::CrossRubberBand,
                                   QwtPicker::AlwaysOn,
                                   canvas());
    plotPicker->setStateMachine(new QwtPickerDragPointMachine());
    plotPicker->setRubberBandPen(QColor(Qt::blue));
    plotPicker->setRubberBand(QwtPicker::CrossRubberBand);
    plotPicker->setTrackerPen(QColor(Qt::black));

    setAutoFillBackground(true);
    setPalette(QPalette(QColor(199, 237, 255))); // Light Blue

    updateWidgetGradient();

    setTitle(tr("LineChart Demonstration for Sensors"));

    /* Legend */
    legend = new QwtLegend();
    legend->setItemMode(QwtLegend::CheckableItem);
    insertLegend(legend, QwtPlot::RightLegend);

    /* Canvas Settings */
    canvas()->setLineWidth(1);
    canvas()->setFrameStyle(QFrame::Box | QFrame::Plain);

    QPalette canvasPalette(Qt::white);
    canvasPalette.setColor(QPalette::Foreground, QColor(133, 190, 232));
    canvas()->setPalette(canvasPalette);

    plotAssay();

    /* Legend */
    connect(this, SIGNAL(legendChecked(QwtPlotItem *, bool)),
            SLOT(showItem(QwtPlotItem *, bool)));
    QwtPlotItemList items = itemList(QwtPlotItem::Rtti_PlotCurve);
    for (int i = 0; i < items.size(); i++)
    {
        if (i == 0)
        {
            QwtLegendItem *legendItem =
                    qobject_cast<QwtLegendItem *>(legend->find(items[i]));
            if (legendItem)
            {
                legendItem->setChecked(true);
            }

            items[i]->setVisible(true);
        }
        else
        {
            items[i]->setVisible(false);
        }
    }
    setAutoReplot(true);
}

void LineChart::updateWidgetGradient()
{
    QPalette pal = palette();

    const QColor buttonColor = pal.color(QPalette::Button);

    QLinearGradient gradient(0, 0, 0, 1);
    gradient.setCoordinateMode(QGradient::StretchToDeviceMode);
    gradient.setColorAt(0.0, Qt::white);
    gradient.setColorAt(0.7, buttonColor);
    gradient.setColorAt(1.0, buttonColor);

    pal.setBrush(QPalette::Window, gradient);
    setPalette(pal);
}

void LineChart::plotAssay()
{
    /* Grid settings */
    gridToPlot = new QwtPlotGrid();
    gridToPlot->enableXMin(true);
    gridToPlot->enableYMin(true);
    gridToPlot->setMajPen(QPen(Qt::gray, 0, Qt::DotLine));
    gridToPlot->setMinPen(QPen(Qt::gray, 0, Qt::DotLine));
    gridToPlot->attach(this);

    /* Axes */
    /* X */
    setAxisTitle(xBottom, tr("The number of days that have passed -->"));
    setAxisScale(xBottom, 0.0, (double)row);

    /* Y */
    setAxisTitle(yLeft, tr("The values of sensors -->"));
    setAxisScale(yLeft, 92.63, 92.7);

    /* Data */
    QPolygonF points[column];

    /* Read Data from Array */
    for(size_t i = 0; i < row; ++i)
    {
        for (size_t j = 0; j < col; ++j)
        {
            points[j].push_back(QPointF((double)i, vectorSensorReadings2DToChart[i][j]));
        }
    }

    /* Colors */
    QColor colors[column];

    /* Setting Colors for dots and charts */
    for(size_t i = 0; i < col; ++i)
    {
        switch (i)
        {
        case 0:
        case 8:
            colors[i].setRgb(255, 0, i * 10); // Red
            break;
        case 1:
        case 9:
            colors[i].setRgb(51, 255, i * 10); // Green
            break;
        case 2:
        case 10:
            colors[i].setRgb(i * 10, 0, 255); // Blue
            break;
        case 3:
        case 11:
            colors[i].setRgb(255, i * 10, 204); // Magenta
            break;
        case 4:
        case 12:
            colors[i].setRgb(255, 255, i * 5); // Yellow
            break;
        case 5:
        case 13:
            colors[i].setRgb(255, 204, i * 5); // Orange
            break;
        case 6:
        case 14:
            colors[i].setRgb(204, 204, i * 5); // Marsh
            break;
        default:
            colors[i] = Qt::black; // Black
            break;
        }
    }

    /* Dots */
    QwtSymbol *symbols[column];

    /* Settings dots */
    for (size_t i = 0; i < col; ++i)
    {
        symbols[i] = new QwtSymbol;
        symbols[i]->setStyle(QwtSymbol::Ellipse);
        symbols[i]->setBrush(QBrush(Qt::white));
        symbols[i]->setPen(QPen(colors[i], 2));
        symbols[i]->setSize(5, 5);
    }

    /* Curves */
    QwtPlotCurve *sensors[column];

    /* Setting curves */
    for (size_t i = 0; i < col; ++i)
    {
        sensors[i] = new QwtPlotCurve(tr("Sensor %1").arg(i+1));
        sensors[i]->setRenderHint(QwtPlotItem::RenderAntialiased, true);
        sensors[i]->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
        sensors[i]->setPen(QPen(colors[i], 2));
        sensors[i]->setSymbol(symbols[i]);
        sensors[i]->setSamples(points[i]);
        sensors[i]->attach(this);
    }

    /* Insert markers */
    //  A horizontal line at y = 92.64
    mY = new QwtPlotMarker();
    mY->setLabel(QString::fromLatin1("y = 92.63"));
    mY->setLabelAlignment(Qt::AlignRight | Qt::AlignTop);
    mY->setLineStyle(QwtPlotMarker::HLine);
    mY->setLinePen(QPen(Qt::black, 0, Qt::DashDotLine));
    mY->setYValue(92.63);
    mY->attach(this);

    //  A vertical line at x = 0
    mX = new QwtPlotMarker();
    mX->setLabel(QString::fromLatin1("x = 0"));
    mX->setLabelAlignment(Qt::AlignLeft | Qt::AlignBottom);
    mX->setLabelOrientation(Qt::Vertical);
    mX->setLineStyle(QwtPlotMarker::VLine);
    mX->setLinePen(QPen(Qt::black, 0, Qt::DashDotLine));
    mX->setXValue(0.0);
    mX->attach(this);
}

void LineChart::resizeEvent(QResizeEvent *event)
{
    QwtPlot::resizeEvent(event);
}

void LineChart::showItem(QwtPlotItem *item, bool on)
{
    item->setVisible(on);
}

void LineChart::exportPlotToImage()
{
#ifndef QT_NO_PRINTER
    QString fileName = "Chart.pdf";
#else
    QString fileName = "Chart.png";
#endif

#ifndef QT_NO_FILEDIALOG
    const QList<QByteArray> imageFormats =
            QImageWriter::supportedImageFormats();

    QStringList filter;
    filter += tr("PDF Documents (*.pdf)");
#ifndef QWT_NO_SVG
    filter += tr("SVG Documents (*.svg)");
#endif
    filter += tr("Postscript Documents (*.ps)");

    if (imageFormats.size() > 0)
    {
        QString imageFilter(tr("Images ("));
        for (int i = 0; i < imageFormats.size(); i++)
        {
            if (i > 0)
                imageFilter += " ";
            imageFilter += "*.";
            imageFilter += imageFormats[i];
        }
        imageFilter += ")";

        filter += imageFilter;
    }

    fileName = QFileDialog::getSaveFileName(
                this, tr("Export File Name"), fileName,
                filter.join(";;"), NULL, QFileDialog::DontConfirmOverwrite);
#endif
    if (!fileName.isEmpty())
    {
        QwtPlotRenderer renderer;
        renderer.setDiscardFlag(QwtPlotRenderer::DiscardBackground, false);

        renderer.renderDocument(this, fileName, QSizeF(300, 200), 85);
    }
}

void LineChart::printPlot()
{
    QPrinter printer(QPrinter::HighResolution);

    QString docName = title().text();
    if (!docName.isEmpty())
    {
        docName.replace(QRegExp(QString::fromLatin1("\n")), tr(" -- "));
        printer.setDocName(docName);
    }

    printer.setCreator(tr("Sensors Chart"));
    printer.setOrientation(QPrinter::Landscape);

    QPrintDialog dialog(&printer);
    if (dialog.exec())
    {
        QwtPlotRenderer renderer;

        if (printer.colorMode() == QPrinter::GrayScale)
        {
            renderer.setDiscardFlag(QwtPlotRenderer::DiscardCanvasBackground);
            renderer.setLayoutFlag(QwtPlotRenderer::FrameWithScales);
        }

        renderer.renderTo(this, printer);
    }
}

LineChart::~LineChart()
{
    /* Empty Destructor */
}

