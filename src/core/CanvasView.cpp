#include "CanvasView.hpp"

#include <algorithm>
#include <QtCore>
#include <QtQml>
#include <QDebug>

using std::max;

CanvasFrame::CanvasFrame(CanvasView *view) :
    QObject(view)
{
    // Do nothing
}

CanvasView::CanvasView(QQuickItem *parent) :
    QQuickItem(parent),
    m_canvasFrame(new CanvasFrame(this)),
    imageItem(0)
{
    // Do nothing
}

CanvasView::~CanvasView()
{
    // Do nothing
}

void CanvasView::setCanvas(Canvas *canvas)
{
    if (m_canvas != canvas)
    {
        m_canvas = canvas;
        if (canvasProvider != 0)
        {
            canvasProvider->setCanvas(canvas);
            refreshCanvas();
        }
        emit canvasChanged();
    }
}

void CanvasView::setZoom(qreal zoom)
{
    zoom = max(zoom, 0.01); // Otherwise we would divide by 0 in the next call
    if (this->zoom() != zoom)
    {
        qreal f = zoom / this->zoom() - 1.0;
        imageItem->setScale(zoom);
        emit zoomChanged();
        emit canvasFrame()->widthChanged();
        emit canvasFrame()->heightChanged();
        emit canvasFrame()->sizeChanged();
        QPointF center(width() / 2.0, height() / 2.0);
        dragBy(f * (canvasFrame()->position() - center));
    }
}

void CanvasView::dragTo(const QPointF &pos)
{
    bool differentX = (imageItem->x() != pos.x());
    bool differentY = (imageItem->y() != pos.y());
    if (differentX || differentY)
    {
        imageItem->setPosition(pos);
        if (differentX)
            emit canvasFrame()->xChanged();
        if (differentY)
            emit canvasFrame()->yChanged();
        emit canvasFrame()->positionChanged();
    }
}

void CanvasView::refreshCanvas()
{
    QVariant oldSource = imageItem->property("source");
    imageItem->setProperty("source", "");
    imageItem->setProperty("source", oldSource);
}

void CanvasView::componentComplete()
{
    QQuickItem::componentComplete();

    QQmlContext *context = QQmlEngine::contextForObject(this);
    QQmlEngine *engine = context->engine();

    QString canvasProviderId = QString().sprintf("canvasview%p", this);
    canvasProvider = new CanvasProvider(m_canvas); // TODO Possible leak
    engine->addImageProvider(canvasProviderId, canvasProvider);

    QQmlComponent imageComponent(engine, QUrl("qrc:/core/canvasview.qml"));
    imageItem = qobject_cast<QQuickItem *>(imageComponent.create(context));
    imageItem->setProperty("source", "image://" + canvasProviderId + "/canvas");
    imageItem->setParent(this);
    imageItem->setParentItem(this);
    m_canvasFrame->imageItem = imageItem;
}
