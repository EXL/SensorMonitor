#include "LevelOneMuChart.h"

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

LevelOneMuChart::LevelOneMuChart(QWidget *parent)
    : QwtPlot(parent)
{
    setAutoReplot(false);

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

    setTitle(tr("Chart of available deviations"));

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
        if (i < 3)
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

void LevelOneMuChart::plotAssay()
{
    /* Grid settings */
    gridToPlot = new QwtPlotGrid();
    gridToPlot->enableXMin(true);
    gridToPlot->enableYMin(true);
    gridToPlot->setMajPen(QPen(Qt::gray, 0, Qt::DotLine));
    gridToPlot->setMinPen(QPen(Qt::gray, 0, Qt::DotLine));
    gridToPlot->attach(this);

    /* Dots and settings dots*/
    symbolMuLower = new QwtSymbol;
    symbolMuLower->setStyle(QwtSymbol::Ellipse);
    symbolMuLower->setBrush(QBrush(Qt::white));
    symbolMuLower->setPen(QPen(Qt::red, 2));
    symbolMuLower->setSize(5, 5);

    symbolMu = new QwtSymbol;
    symbolMu->setStyle(QwtSymbol::Ellipse);
    symbolMu->setBrush(QBrush(Qt::white));
    symbolMu->setPen(QPen(Qt::green, 2));
    symbolMu->setSize(5, 5);

    symbolMuUpper = new QwtSymbol;
    symbolMuUpper->setStyle(QwtSymbol::Ellipse);
    symbolMuUpper->setBrush(QBrush(Qt::white));
    symbolMuUpper->setPen(QPen(Qt::blue, 2));
    symbolMuUpper->setSize(5, 5);

    /* Curves */
    curveMuLower = new QwtPlotCurve(tr("Mu[Low.Lim]"));
    curveMu = new QwtPlotCurve(tr("Mu"));
    curveMuUpper = new QwtPlotCurve(tr("Mu[Upp.Lim]"));

    /* Setting curves */
    curveMuLower->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curveMuLower->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    curveMuLower->setPen(QPen(Qt::red, 2));
    curveMuLower->setSymbol(symbolMuLower);
    curveMuLower->attach(this);

    curveMu->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curveMu->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    curveMu->setPen(QPen(Qt::green, 2));
    curveMu->setSymbol(symbolMu);
    curveMu->attach(this);

    curveMuUpper->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curveMuUpper->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    curveMuUpper->setPen(QPen(Qt::blue, 2));
    curveMuUpper->setSymbol(symbolMuUpper);
    curveMuUpper->attach(this);

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

    /* Axes */
    /* X */
    setAxisTitle(xBottom, tr("Mu -->"));
    setAxisScale(xBottom, 370.625, 370.651);

    /* Y */
    setAxisTitle(yLeft, tr("Alpha -->"));
    setAxisScale(yLeft, 0, 6e-05);
}

void LevelOneMuChart::readDataOfVectors(bool qFirst,
                                        const QVector<double> &vectorMuLower,
                                        const QVector<double> &vectorMu,
                                        const QVector<double> &vectorMuUpper,
                                        const QVector<QString> &vectorAlpha)
{
    size_t row = vectorMu.size();

    /* Data */
    QPolygonF points_s1;
    QPolygonF points_s2;
    QPolygonF points_s3;

    if(qFirst)
    {
        points_s1.clear();
        points_s2.clear();
        points_s3.clear();
    }

    /* Read Data from Array */
    for(size_t i = 0; i < row; ++i)
    {
        points_s1.push_back(QPointF(vectorMuLower[i], vectorAlpha[i].toDouble()));
        points_s2.push_back(QPointF(vectorMu[i], vectorAlpha[i].toDouble()));
        points_s3.push_back(QPointF(vectorMuUpper[i], vectorAlpha[i].toDouble()));
    }

    curveMuLower->setSamples(points_s1);
    curveMu->setSamples(points_s2);
    curveMuUpper->setSamples(points_s3);
}

void LevelOneMuChart::updateWidgetGradient()
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

void LevelOneMuChart::resizeEvent(QResizeEvent *event)
{
    QwtPlot::resizeEvent(event);
}

void LevelOneMuChart::showItem(QwtPlotItem *item, bool on)
{
    item->setVisible(on);
}

void LevelOneMuChart::exportLevelOnePlotToImage()
{

#ifndef QT_NO_PRINTER
    QString fileName = "MuChartOneLevel.pdf";
#else
    QString fileName = "MuChartOneLevel.png";
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

void LevelOneMuChart::printLevelOnePlot()
{
    QPrinter printer(QPrinter::HighResolution);

    QString docName = title().text();
    if (!docName.isEmpty())
    {
        docName.replace(QRegExp(QString::fromLatin1("\n")), tr(" -- "));
        printer.setDocName(docName);
    }

    printer.setCreator(tr("Levels One Mu Chart"));
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

LevelOneMuChart::~LevelOneMuChart()
{
    /* Empty destructor */
}
