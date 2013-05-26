#include "LevelOneChart.h"

#include <QFileDialog>
#include <QImageWriter>
#include <QPrintDialog>
#include <QPrinter>

#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_picker_machine.h>
#include <qwt_legend.h>
#include <qwt_legend_item.h>
#include <qwt_symbol.h>
#include <qwt_plot_renderer.h>

LevelOneChart::LevelOneChart(QWidget *parent)
    : QwtPlot(parent)
{
    setAutoReplot(false);

    /* Panning with the left mouse button */
    (void)new QwtPlotPanner(canvas());

    /* Zoom in/out with the wheel */
    (void)new QwtPlotMagnifier(canvas());

    /* Strings */
    strR = tr("Real");
    strF = tr("Forecast");

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

    setTitle(tr("Chart of of phase coordinates"));

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
        if (i == 0 || i == 1)
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

void LevelOneChart::plotAssay()
{
    /* Grid settings */
    gridToPlot = new QwtPlotGrid();
    gridToPlot->enableXMin(true);
    gridToPlot->enableYMin(true);
    gridToPlot->setMajPen(QPen(Qt::gray, 0, Qt::DotLine));
    gridToPlot->setMinPen(QPen(Qt::gray, 0, Qt::DotLine));
    gridToPlot->attach(this);

    /* Dots and settings dots*/
    symbolR = new QwtSymbol;
    symbolR->setStyle(QwtSymbol::Ellipse);
    symbolR->setBrush(QBrush(Qt::white));
    symbolR->setPen(QPen(Qt::blue, 2));
    symbolR->setSize(5, 5);

    symbolF = new QwtSymbol;
    symbolF->setStyle(QwtSymbol::Ellipse);
    symbolF->setBrush(QBrush(Qt::white));
    symbolF->setPen(QPen(Qt::red, 2));
    symbolF->setSize(5, 5);

    forecastSymbol = new QwtSymbol;
    forecastSymbol->setStyle(QwtSymbol::Diamond);
    forecastSymbol->setBrush(QBrush(Qt::yellow));
    forecastSymbol->setPen(QPen(Qt::red, 2));
    forecastSymbol->setSize(10, 10);

    /* Curves */
    curveR = new QwtPlotCurve(strR);
    curveF = new QwtPlotCurve(strF);

    /* Setting curves */
    curveR->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curveR->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    curveR->setPen(QPen(Qt::blue, 2));
    curveR->setSymbol(symbolR);
    curveR->attach(this);

    curveF->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curveF->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    curveF->setPen(QPen(Qt::red, 2));
    curveF->setSymbol(symbolF);
    curveF->attach(this);

    /* Insert markers */
    //  A horizontal line at y = 0.000038
    mY = new QwtPlotMarker();
    mY->setLabel(QString::fromLatin1("y = 0.000038"));
    mY->setLabelAlignment(Qt::AlignRight | Qt::AlignTop);
    mY->setLineStyle(QwtPlotMarker::HLine);
    mY->setLinePen(QPen(Qt::black, 0, Qt::DashDotLine));
    mY->setYValue(38e-06);
    mY->attach(this);

    //  A vertical line at x = 370.638
    mX = new QwtPlotMarker();
    mX->setLabel(QString::fromLatin1("x = 370.638"));
    mX->setLabelAlignment(Qt::AlignLeft | Qt::AlignBottom);
    mX->setLabelOrientation(Qt::Vertical);
    mX->setLineStyle(QwtPlotMarker::VLine);
    mX->setLinePen(QPen(Qt::black, 0, Qt::DashDotLine));
    mX->setXValue(370.638);
    mX->attach(this);

    forecastDot = new QwtPlotMarker();
    forecastDot->setSymbol(forecastSymbol);
    forecastDot->attach(this);

    /* Axes */
    /* X */
    setAxisTitle(xBottom, tr("Mu -->"));
    setAxisScale(xBottom, 370.625, 370.651);

    /* Y */
    setAxisTitle(yLeft, tr("Alpha -->"));
    setAxisScale(yLeft, 0, 6e-05);
}

void LevelOneChart::readDataOfVectors(const QVector<double> &vectorMu,
                              const QVector<double> &vectorMuForecast,
                              const QVector<QString> &vectorAlpha,
                              const QVector<QString> &vectorAlphaForecast)
{
    size_t row = vectorMu.size();

    /* Data */
    QPolygonF points_r;
    QPolygonF points_f;

    /* Read Data from Array */
    for(size_t i = 0; i < row; ++i)
    {
        points_r.push_back(QPointF(vectorMu[i], vectorAlpha[i].toDouble()));
    }

    for(size_t i = 0; i <= row; ++i)
    {
        points_f.push_back(QPointF(vectorMuForecast[i], vectorAlphaForecast[i].toDouble()));
    }

    forecastDot->setValue(QPointF(vectorMuForecast[row], vectorAlphaForecast[row].toDouble()));

    curveR->setSamples(points_r);
    curveF->setSamples(points_f);
}

void LevelOneChart::updateWidgetGradient()
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

void LevelOneChart::resizeEvent(QResizeEvent *event)
{
    QwtPlot::resizeEvent(event);
}

void LevelOneChart::showItem(QwtPlotItem *item, bool on)
{
    if (item->title().text() == strF)
    {
        forecastDot->setVisible(on);
    }
    item->setVisible(on);
}

void LevelOneChart::exportLevelOnePlotToImage()
{
#ifndef QT_NO_PRINTER
    QString fileName = "ChartOneLevel.pdf";
#else
    QString fileName = "ChartOneLevel.png";
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

void LevelOneChart::printLevelOnePlot()
{
    QPrinter printer(QPrinter::HighResolution);

    QString docName = title().text();
    if (!docName.isEmpty())
    {
        docName.replace(QRegExp(QString::fromLatin1("\n")), tr(" -- "));
        printer.setDocName(docName);
    }

    printer.setCreator(tr("OneLevelChart"));
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

LevelOneChart::~LevelOneChart()
{
    /* Empty destructor */
}
