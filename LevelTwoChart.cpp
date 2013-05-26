#include "LevelTwoChart.h"

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

LevelTwoChart::LevelTwoChart(QWidget *parent)
    : QwtPlot(parent)
{
    setAutoReplot(false);

    /* Panning with the left mouse button */
    (void)new QwtPlotPanner(canvas());

    /* Zoom in/out with the wheel */
    (void)new QwtPlotMagnifier(canvas());

    /* Strings */
    strRA = tr("[A] Real");
    strFA = tr("[A] Forecast");
    strRB = tr("[B] Real");
    strFB = tr("[B] Forecast");
    strRC = tr("[C] Real");
    strFC = tr("[C] Forecast");

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
            forecastDotB->setVisible(false);
            forecastDotC->setVisible(false);
            items[i]->setVisible(false);
        }
    }
    setAutoReplot(true);
}

void LevelTwoChart::plotAssay()
{
    /* Grid settings */
    gridToPlot = new QwtPlotGrid();
    gridToPlot->enableXMin(true);
    gridToPlot->enableYMin(true);
    gridToPlot->setMajPen(QPen(Qt::gray, 0, Qt::DotLine));
    gridToPlot->setMinPen(QPen(Qt::gray, 0, Qt::DotLine));
    gridToPlot->attach(this);

    /* Dots and settings dots*/
    symbolRA = new QwtSymbol;
    symbolRA->setStyle(QwtSymbol::Ellipse);
    symbolRA->setBrush(QBrush(Qt::white));
    symbolRA->setPen(QPen(QColor(255, 0, 0), 2));
    symbolRA->setSize(5, 5);

    symbolFA = new QwtSymbol;
    symbolFA->setStyle(QwtSymbol::Ellipse);
    symbolFA->setBrush(QBrush(Qt::white));
    symbolFA->setPen(QPen(QColor(255, 204, 153), 2));
    symbolFA->setSize(5, 5);

    symbolRB = new QwtSymbol;
    symbolRB->setStyle(QwtSymbol::Ellipse);
    symbolRB->setBrush(QBrush(Qt::white));
    symbolRB->setPen(QPen(QColor(0, 255, 0), 2));
    symbolRB->setSize(5, 5);

    symbolFB = new QwtSymbol;
    symbolFB->setStyle(QwtSymbol::Ellipse);
    symbolFB->setBrush(QBrush(Qt::white));
    symbolFB->setPen(QPen(QColor(204, 255, 204), 2));
    symbolFB->setSize(5, 5);

    symbolRC = new QwtSymbol;
    symbolRC->setStyle(QwtSymbol::Ellipse);
    symbolRC->setBrush(QBrush(Qt::white));
    symbolRC->setPen(QPen(QColor(0, 0, 255), 2));
    symbolRC->setSize(5, 5);

    symbolFC = new QwtSymbol;
    symbolFC->setStyle(QwtSymbol::Ellipse);
    symbolFC->setBrush(QBrush(Qt::white));
    symbolFC->setPen(QPen(QColor(153, 204, 254), 2));
    symbolFC->setSize(5, 5);

    forecastSymbolA = new QwtSymbol;
    forecastSymbolA->setStyle(QwtSymbol::Diamond);
    forecastSymbolA->setBrush(QBrush(Qt::yellow));
    forecastSymbolA->setPen(QPen(QColor(255, 204, 153), 2));
    forecastSymbolA->setSize(10, 10);

    forecastSymbolB = new QwtSymbol;
    forecastSymbolB->setStyle(QwtSymbol::Diamond);
    forecastSymbolB->setBrush(QBrush(Qt::yellow));
    forecastSymbolB->setPen(QPen(QColor(204, 255, 204), 2));
    forecastSymbolB->setSize(10, 10);

    forecastSymbolC = new QwtSymbol;
    forecastSymbolC->setStyle(QwtSymbol::Diamond);
    forecastSymbolC->setBrush(QBrush(Qt::yellow));
    forecastSymbolC->setPen(QPen(QColor(153, 204, 254), 2));
    forecastSymbolC->setSize(10, 10);

    /* Curves */
    curveRA = new QwtPlotCurve(strRA);
    curveFA = new QwtPlotCurve(strFA);
    curveRB = new QwtPlotCurve(strRB);
    curveFB = new QwtPlotCurve(strFB);
    curveRC = new QwtPlotCurve(strRC);
    curveFC = new QwtPlotCurve(strFC);

    /* Setting curves */
    curveRA->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curveRA->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    curveRA->setPen(QPen(QColor(255, 0, 0), 2));
    curveRA->setSymbol(symbolRA);
    curveRA->attach(this);

    curveFA->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curveFA->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    curveFA->setPen(QPen(QColor(255, 204, 153), 2));
    curveFA->setSymbol(symbolFA);
    curveFA->attach(this);

    curveRB->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curveRB->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    curveRB->setPen(QPen(QColor(0, 255, 0), 2));
    curveRB->setSymbol(symbolRB);
    curveRB->attach(this);

    curveFB->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curveFB->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    curveFB->setPen(QPen(QColor(204, 255, 204), 2));
    curveFB->setSymbol(symbolFB);
    curveFB->attach(this);

    curveRC->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curveRC->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    curveRC->setPen(QPen(QColor(0, 0, 255), 2));
    curveRC->setSymbol(symbolRC);
    curveRC->attach(this);

    curveFC->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curveFC->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    curveFC->setPen(QPen(QColor(153, 204, 255), 2));
    curveFC->setSymbol(symbolFC);
    curveFC->attach(this);

    /* Insert markers */
    //  A horizontal line at y = 0.000038
    mY = new QwtPlotMarker();
    mY->setLabel(QString::fromLatin1("y = 0.000038"));
    mY->setLabelAlignment(Qt::AlignRight | Qt::AlignTop);
    mY->setLineStyle(QwtPlotMarker::HLine);
    mY->setLinePen(QPen(Qt::black, 0, Qt::DashDotLine));
    mY->setYValue(38e-06);
    mY->attach(this);

    //  A vertical line at x = 226.958
    mX = new QwtPlotMarker();
    mX->setLabel(QString::fromLatin1("x = 226.958"));
    mX->setLabelAlignment(Qt::AlignLeft | Qt::AlignBottom);
    mX->setLabelOrientation(Qt::Vertical);
    mX->setLineStyle(QwtPlotMarker::VLine);
    mX->setLinePen(QPen(Qt::black, 0, Qt::DashDotLine));
    mX->setXValue(226.958);
    mX->attach(this);

    forecastDotA = new QwtPlotMarker();
    forecastDotA->setSymbol(forecastSymbolA);
    forecastDotA->attach(this);

    forecastDotB = new QwtPlotMarker();
    forecastDotB->setSymbol(forecastSymbolB);
    forecastDotB->attach(this);

    forecastDotC = new QwtPlotMarker();
    forecastDotC->setSymbol(forecastSymbolC);
    forecastDotC->attach(this);

    /* Axes */
    /* X */
    setAxisTitle(xBottom, tr("Mu -->"));
    setAxisScale(xBottom, 226.945, 226.97);

    /* Y */
    setAxisTitle(yLeft, tr("Alpha -->"));
    setAxisScale(yLeft, (-1e-05), 7e-05);
}

void LevelTwoChart::readDataOfVectors(const QVector<double> &vectorMuA,
                                      const QVector<double> &vectorMuForecastA,
                                      const QVector<QString> &vectorAlphaA,
                                      const QVector<QString> &vectorAlphaForecastA,
                                      const QVector<double> &vectorMuB,
                                      const QVector<double> &vectorMuForecastB,
                                      const QVector<QString> &vectorAlphaB,
                                      const QVector<QString> &vectorAlphaForecastB,
                                      const QVector<double> &vectorMuC,
                                      const QVector<double> &vectorMuForecastC,
                                      const QVector<QString> &vectorAlphaC,
                                      const QVector<QString> &vectorAlphaForecastC)
{
    size_t row = vectorMuA.size();

    /* Data */
    QPolygonF points_ra;
    QPolygonF points_fa;
    QPolygonF points_rb;
    QPolygonF points_fb;
    QPolygonF points_rc;
    QPolygonF points_fc;

    /* Read Data from Array */
    for(size_t i = 0; i < row; ++i)
    {
        points_ra.push_back(QPointF(vectorMuA[i], vectorAlphaA[i].toDouble()));
        points_rb.push_back(QPointF(vectorMuB[i], vectorAlphaB[i].toDouble()));
        points_rc.push_back(QPointF(vectorMuC[i], vectorAlphaC[i].toDouble()));
    }

    for(size_t i = 0; i <= row; ++i)
    {
        points_fa.push_back(QPointF(vectorMuForecastA[i], vectorAlphaForecastA[i].toDouble()));
        points_fb.push_back(QPointF(vectorMuForecastB[i], vectorAlphaForecastB[i].toDouble()));
        points_fc.push_back(QPointF(vectorMuForecastC[i], vectorAlphaForecastC[i].toDouble()));
    }

    forecastDotA->setValue(QPointF(vectorMuForecastA[row], vectorAlphaForecastA[row].toDouble()));
    forecastDotB->setValue(QPointF(vectorMuForecastB[row], vectorAlphaForecastB[row].toDouble()));
    forecastDotC->setValue(QPointF(vectorMuForecastC[row], vectorAlphaForecastC[row].toDouble()));

    curveRA->setSamples(points_ra);
    curveFA->setSamples(points_fa);
    curveRB->setSamples(points_rb);
    curveFB->setSamples(points_fb);
    curveRC->setSamples(points_rc);
    curveFC->setSamples(points_fc);
}

void LevelTwoChart::updateWidgetGradient()
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

void LevelTwoChart::resizeEvent(QResizeEvent *event)
{
    QwtPlot::resizeEvent(event);
}

void LevelTwoChart::showItem(QwtPlotItem *item, bool on)
{
    if(item->title().text() == strRA && on)
    {
        setAxisScale(xBottom, 226.945, 226.97);
    }
    else if (item->title().text() == strFA)
    {
        forecastDotA->setVisible(on);
        if (on)
        {
            setAxisScale(xBottom, 226.945, 226.97);
        }
    }
    else if(item->title().text() == strRB && on)
    {
        setAxisScale(xBottom, 226.945, 226.97);
    }
    else if(item->title().text() == strFB)
    {
        forecastDotB->setVisible(on);
        if (on)
        {
            setAxisScale(xBottom, 226.945, 226.97);
        }
    }
    else if (item->title().text() == strRC && on)
    {
        setAxisScale(xBottom, 185.325, 185.355);
    }
    else if (item->title().text() == strFC)
    {
        forecastDotC->setVisible(on);

        if (on)
        {
            setAxisScale(xBottom, 185.325, 185.355);
        }
    }
    item->setVisible(on);
}

void LevelTwoChart::exportLevelTwoPlotToImage()
{
#ifndef QT_NO_PRINTER
    QString fileName = "ChartTwoLevel.pdf";
#else
    QString fileName = "ChartTwoLevel.png";
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

void LevelTwoChart::printLevelTwoPlot()
{
    QPrinter printer(QPrinter::HighResolution);

    QString docName = title().text();
    if (!docName.isEmpty())
    {
        docName.replace(QRegExp(QString::fromLatin1("\n")), tr(" -- "));
        printer.setDocName(docName);
    }

    printer.setCreator(tr("TwoLevelChart"));
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

LevelTwoChart::~LevelTwoChart()
{
    /* Empty destructor */
}
