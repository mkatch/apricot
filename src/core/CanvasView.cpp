#include "CanvasView.hpp"

#include <algorithm>
using std::max;

#include "ApricotUtils.hpp"
#include "Canvas.hpp"

CanvasView::CanvasView(QWidget *parent) :
    QWidget(parent),
    graphicsView(new QGraphicsView(this)),
    graphicsScene(new QGraphicsScene(this)),
    placeholderPixmap(256, 256),
    m_canvas(nullptr)
{
    canvasItem = graphicsScene->addPixmap(placeholderPixmap);
    graphicsView->setScene(graphicsScene);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    layOut();
}

CanvasView::~CanvasView()
{
    // Do nothing
}

void CanvasView::setCanvas(const Canvas *canvas)
{
    if (m_canvas == canvas) {
        return;
    }

    m_canvas = canvas;
    canvasItem->setPixmap(m_canvas != nullptr ? m_canvas->toPixmap() : placeholderPixmap);

    emit canvasChanged();
}

void CanvasView::setScale(qreal scale)
{
    if (canvasItem->scale() == scale) {
        return;
    }

    canvasItem->setScale(scale);
    emit scaleChanged();
    emit transformChanged();
}

void CanvasView::setTranslation(const QPointF &translation)
{
    if (canvasItem->pos() == translation) {
        return;
    }

    canvasItem->setPos(translation);
    emit translationChanged();
    emit transformChanged();
}

QTransform CanvasView::transform() const
{
    return QTransform().scale(scale(), scale()).translate(translation().x(), translation().y());
}

QPointF CanvasView::mapToCanvas(const QPointF &point) const
{
    return (point - translation()) / scale();
}

QPointF CanvasView::mapFromCanvas(const QPointF &point) const
{
    return point * scale() + translation();
}

void CanvasView::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    layOut();
}

void CanvasView::layOut()
{
    graphicsView->resize(size());
    graphicsView->setSceneRect(1, 1, width() - 1, height() - 1);
}
