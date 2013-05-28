#include "LevelTwoMuChart.h"

#include <QFileDialog>
#include <QImageWriter>
#include <QPrintDialog>
#include <QPrinter>

#ifdef _DEBUG
#include <QDebug>
#endif

#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_picker_machine.h>
#include <qwt_legend.h>
#include <qwt_legend_item.h>
#include <qwt_symbol.h>
#include <qwt_plot_renderer.h>

LevelTwoMuChart::LevelTwoMuChart(QWidget *parent)
    : QwtPlot(parent)
{
    setAutoReplot(false);

    /* Panning with the left mouse button */
    (void)new QwtPlotPanner(canvas());

    /* Zoom in/out with the wheel */
    (void)new QwtPlotMagnifier(canvas());

    /* Strings */
    strMuLowerBlockA = tr("[A] Mu[Low.Lim]");
    strMuBlockA = tr("[A] Mu");
    strMuUpperBlockA = tr("[A] Mu[Upp.Lim]");
    strMuLowerBlockB = tr("[B] Mu[Low.Lim]");
    strMuBlockB = tr("[B] Mu");
    strMuUpperBlockB = tr("[B] Mu[Upp.Lim]");
    strMuLowerBlockC = tr("[C] Mu[Low.Lim]");
    strMuBlockC = tr("[C] Mu");
    strMuUpperBlockC = tr("[C] Mu[Upp.Lim]");

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

void LevelTwoMuChart::plotAssay()
{
    /* Grid settings */
    gridToPlot = new QwtPlotGrid();
    gridToPlot->enableXMin(true);
    gridToPlot->enableYMin(true);
    gridToPlot->setMajPen(QPen(Qt::gray, 0, Qt::DotLine));
    gridToPlot->setMinPen(QPen(Qt::gray, 0, Qt::DotLine));
    gridToPlot->attach(this);

    /* Dots and settings dots*/
    symbolMuLowerBlockA = new QwtSymbol;
    symbolMuLowerBlockA->setStyle(QwtSymbol::Ellipse);
    symbolMuLowerBlockA->setBrush(QBrush(Qt::white));
    symbolMuLowerBlockA->setPen(QPen(QColor(255, 0, 0), 1));
    symbolMuLowerBlockA->setSize(3, 3);

    symbolMuBlockA = new QwtSymbol;
    symbolMuBlockA->setStyle(QwtSymbol::Ellipse);
    symbolMuBlockA->setBrush(QBrush(Qt::white));
    symbolMuBlockA->setPen(QPen(QColor(180, 0, 0), 1));
    symbolMuBlockA->setSize(3, 3);

    symbolMuUpperBlockA = new QwtSymbol;
    symbolMuUpperBlockA->setStyle(QwtSymbol::Ellipse);
    symbolMuUpperBlockA->setBrush(QBrush(Qt::white));
    symbolMuUpperBlockA->setPen(QPen(QColor(102, 0, 0), 1));
    symbolMuUpperBlockA->setSize(3, 3);

    symbolMuLowerBlockB = new QwtSymbol;
    symbolMuLowerBlockB->setStyle(QwtSymbol::Ellipse);
    symbolMuLowerBlockB->setBrush(QBrush(Qt::white));
    symbolMuLowerBlockB->setPen(QPen(QColor(0, 255, 0), 1));
    symbolMuLowerBlockB->setSize(3, 3);

    symbolMuBlockB = new QwtSymbol;
    symbolMuBlockB->setStyle(QwtSymbol::Ellipse);
    symbolMuBlockB->setBrush(QBrush(Qt::white));
    symbolMuBlockB->setPen(QPen(QColor(0, 180, 0), 1));
    symbolMuBlockB->setSize(3, 3);

    symbolMuUpperBlockB = new QwtSymbol;
    symbolMuUpperBlockB->setStyle(QwtSymbol::Ellipse);
    symbolMuUpperBlockB->setBrush(QBrush(Qt::white));
    symbolMuUpperBlockB->setPen(QPen(QColor(0, 102, 0), 1));
    symbolMuUpperBlockB->setSize(3, 3);

    symbolMuLowerBlockC = new QwtSymbol;
    symbolMuLowerBlockC->setStyle(QwtSymbol::Ellipse);
    symbolMuLowerBlockC->setBrush(QBrush(Qt::white));
    symbolMuLowerBlockC->setPen(QPen(QColor(0, 0, 255), 1));
    symbolMuLowerBlockC->setSize(3, 3);

    symbolMuBlockC = new QwtSymbol;
    symbolMuBlockC->setStyle(QwtSymbol::Ellipse);
    symbolMuBlockC->setBrush(QBrush(Qt::white));
    symbolMuBlockC->setPen(QPen(QColor(0, 0, 180), 1));
    symbolMuBlockC->setSize(3, 3);

    symbolMuUpperBlockC = new QwtSymbol;
    symbolMuUpperBlockC->setStyle(QwtSymbol::Ellipse);
    symbolMuUpperBlockC->setBrush(QBrush(Qt::white));
    symbolMuUpperBlockC->setPen(QPen(QColor(0, 0, 102), 1));
    symbolMuUpperBlockC->setSize(3, 3);

    /* Curves */
    curveMuLowerBlockA = new QwtPlotCurve(strMuLowerBlockA);
    curveMuBlockA = new QwtPlotCurve(strMuBlockA);
    curveMuUpperBlockA = new QwtPlotCurve(strMuUpperBlockA);
    curveMuLowerBlockB = new QwtPlotCurve(strMuLowerBlockB);
    curveMuBlockB = new QwtPlotCurve(strMuBlockB);
    curveMuUpperBlockB = new QwtPlotCurve(strMuUpperBlockB);
    curveMuLowerBlockC = new QwtPlotCurve(strMuLowerBlockC);
    curveMuBlockC = new QwtPlotCurve(strMuBlockC);
    curveMuUpperBlockC = new QwtPlotCurve(strMuUpperBlockC);

    /* Setting curves */
    curveMuLowerBlockA->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curveMuLowerBlockA->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    curveMuLowerBlockA->setPen(QPen(QColor(255, 0, 0), 1));
    curveMuLowerBlockA->setSymbol(symbolMuLowerBlockA);
    curveMuLowerBlockA->attach(this);

    curveMuBlockA->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curveMuBlockA->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    curveMuBlockA->setPen(QPen(QColor(180, 0, 0), 1));
    curveMuBlockA->setSymbol(symbolMuBlockA);
    curveMuBlockA->attach(this);

    curveMuUpperBlockA->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curveMuUpperBlockA->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    curveMuUpperBlockA->setPen(QPen(QColor(102, 0, 0), 1));
    curveMuUpperBlockA->setSymbol(symbolMuUpperBlockA);
    curveMuUpperBlockA->attach(this);

    curveMuLowerBlockB->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curveMuLowerBlockB->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    curveMuLowerBlockB->setPen(QPen(QColor(0, 255, 0), 1));
    curveMuLowerBlockB->setSymbol(symbolMuLowerBlockB);
    curveMuLowerBlockB->attach(this);

    curveMuBlockB->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curveMuBlockB->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    curveMuBlockB->setPen(QPen(QColor(0, 180, 0), 1));
    curveMuBlockB->setSymbol(symbolMuBlockB);
    curveMuBlockB->attach(this);

    curveMuUpperBlockB->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curveMuUpperBlockB->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    curveMuUpperBlockB->setPen(QPen(QColor(0, 102, 0), 1));
    curveMuUpperBlockB->setSymbol(symbolMuUpperBlockB);
    curveMuUpperBlockB->attach(this);

    curveMuLowerBlockC->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curveMuLowerBlockC->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    curveMuLowerBlockC->setPen(QPen(QColor(0, 0, 255), 1));
    curveMuLowerBlockC->setSymbol(symbolMuLowerBlockC);
    curveMuLowerBlockC->attach(this);

    curveMuBlockC->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curveMuBlockC->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    curveMuBlockC->setPen(QPen(QColor(0, 0, 180), 1));
    curveMuBlockC->setSymbol(symbolMuBlockC);
    curveMuBlockC->attach(this);

    curveMuUpperBlockC->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curveMuUpperBlockC->setLegendAttribute(QwtPlotCurve::LegendShowLine, true);
    curveMuUpperBlockC->setPen(QPen(QColor(0, 0, 102), 1));
    curveMuUpperBlockC->setSymbol(symbolMuUpperBlockC);
    curveMuUpperBlockC->attach(this);

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

    /* Axes */
    /* X */
    setAxisTitle(xBottom, tr("Mu -->"));
    setAxisScale(xBottom, 226.9345, 226.9775);

    /* Y */
    setAxisTitle(yLeft, tr("Alpha -->"));
    setAxisScale(yLeft, (-2e-05), 8e-05);
}

void LevelTwoMuChart::readDataOfVectors(const QVector<QString> &vectorAlpha,
                                        const QVector<double> &vectorMuLowerBlockA,
                                        const QVector<double> &vectorMuBlockA,
                                        const QVector<double> &vectorMuUpperBlockA,
                                        const QVector<double> &vectorMuLowerBlockB,
                                        const QVector<double> &vectorMuBlockB,
                                        const QVector<double> &vectorMuUpperBlockB,
                                        const QVector<double> &vectorMuLowerBlockC,
                                        const QVector<double> &vectorMuBlockC,
                                        const QVector<double> &vectorMuUpperBlockC)
{
    size_t row = vectorMuBlockA.size();

    /* Data */
    QPolygonF points_LA;
    QPolygonF points_A;
    QPolygonF points_UA;
    QPolygonF points_LB;
    QPolygonF points_B;
    QPolygonF points_UB;
    QPolygonF points_LC;
    QPolygonF points_C;
    QPolygonF points_UC;

#ifdef _DEBUG
    qDebug() << "[LEVEL 2] Size Of points_A:" << points_A.size();
#endif

    /* Read Data from Array */
    for(size_t i = 0; i < row; ++i)
    {
        points_LA.push_back(QPointF(vectorMuLowerBlockA[i], vectorAlpha[i].toDouble()));
        points_A.push_back(QPointF(vectorMuBlockA[i], vectorAlpha[i].toDouble()));
        points_UA.push_back(QPointF(vectorMuUpperBlockA[i], vectorAlpha[i].toDouble()));

        points_LB.push_back(QPointF(vectorMuLowerBlockB[i], vectorAlpha[i].toDouble()));
        points_B.push_back(QPointF(vectorMuBlockB[i], vectorAlpha[i].toDouble()));
        points_UB.push_back(QPointF(vectorMuUpperBlockB[i], vectorAlpha[i].toDouble()));

        points_LC.push_back(QPointF(vectorMuLowerBlockC[i], vectorAlpha[i].toDouble()));
        points_C.push_back(QPointF(vectorMuBlockC[i], vectorAlpha[i].toDouble()));
        points_UC.push_back(QPointF(vectorMuUpperBlockC[i], vectorAlpha[i].toDouble()));
    }

    curveMuLowerBlockA->setSamples(points_LA);
    curveMuBlockA->setSamples(points_A);
    curveMuUpperBlockA->setSamples(points_UA);
    curveMuLowerBlockB->setSamples(points_LB);
    curveMuBlockB->setSamples(points_B);
    curveMuUpperBlockB->setSamples(points_UB);
    curveMuLowerBlockC->setSamples(points_LC);
    curveMuBlockC->setSamples(points_C);
    curveMuUpperBlockC->setSamples(points_UC);

#ifdef _DEBUG
    qDebug() << "[LEVEL 2] Size Of points_A:" << points_A.size();
#endif
}

void LevelTwoMuChart::resizeEvent(QResizeEvent *event)
{
    QwtPlot::resizeEvent(event);
}

void LevelTwoMuChart::showItem(QwtPlotItem *item, bool on)
{
    bool qScale = (item->title().text() == strMuLowerBlockC
            || item->title().text() == strMuUpperBlockC
            || item->title().text() == strMuBlockC);

    if(qScale)
    {
        setAxisScale(yLeft, (-2e-05), 8e-05);
        setAxisScale(xBottom, 185.325, 185.355);
        item->setVisible(on);
        return;
    }

    setAxisScale(yLeft, (-2e-05), 8e-05);
    setAxisScale(xBottom, 226.9345, 226.9775);
    item->setVisible(on);
}

void LevelTwoMuChart::exportLevelTwoPlotToImage()
{
#ifndef QT_NO_PRINTER
    QString fileName = "MuChartTwoLevel.pdf";
#else
    QString fileName = "MuChartTwoLevel.png";
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

void LevelTwoMuChart::printLevelTwoPlot()
{
    QPrinter printer(QPrinter::HighResolution);

    QString docName = title().text();
    if (!docName.isEmpty())
    {
        docName.replace(QRegExp(QString::fromLatin1("\n")), tr(" -- "));
        printer.setDocName(docName);
    }

    printer.setCreator(tr("LevelTwoMuChart"));
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

void LevelTwoMuChart::updateWidgetGradient()
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

LevelTwoMuChart::~LevelTwoMuChart()
{
    /* Empty Destructor */
}
