#include "CanvasView.hpp"
#include "ui_CanvasView.h"

#include <QtWidgets>

#include <algorithm>
using std::max;

#include "Canvas.hpp"

CanvasFrame::CanvasFrame(QGraphicsPixmapItem *pixmapItem) :
    pixmapItem(pixmapItem)
{
    // Do nothing
}

CanvasView::CanvasView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CanvasView),
    placeholderPixmap(1200, 23),
    pixmapItem(scene.addPixmap(placeholderPixmap)),
    m_canvas(nullptr),
    m_canvasFrame(pixmapItem)
{
    ui->setupUi(this);
    ui->graphicsView->setScene(&scene);

    scene.addRect(10, 10, 20, 30);
}

CanvasView::~CanvasView()
{
    delete ui;
}

void CanvasView::setCanvas(Canvas *canvas)
{
    if (m_canvas == canvas) {
        return;
    }

    qreal oldX = canvasFrame()->x();
    qreal oldY = canvasFrame()->y();
    qreal oldWidth = canvasFrame()->width();
    qreal oldHeight = canvasFrame()->height();

    if (m_canvas == nullptr) {
        pixmapItem->setPixmap(placeholderPixmap);
    }
    else {
        pixmapItem->setPixmap(m_canvas->toPixmap());
    }

    emit canvasChanged();

    bool differentX = (canvasFrame()->x() != oldX);
    bool differentY = (canvasFrame()->y() != oldY);
    bool differentWidth = (canvasFrame()->width() != oldWidth);
    bool differentHeight = (canvasFrame()->height() != oldHeight);

    if (differentX) {
        emit canvasFrame()->xChanged();
    }
    if (differentY) {
        emit canvasFrame()->yChanged();
    }
    if (differentX || differentY) {
        emit canvasFrame()->positionChanged();
    }
    if (differentWidth) {
        emit canvasFrame()->widthChanged();
    }
    if (differentHeight) {
        emit canvasFrame()->heightChanged();
    }
    if (differentWidth || differentHeight) {
        emit canvasFrame()->sizeChanged();
    }

    ui->graphicsView->invalidateScene();
}


void CanvasView::setZoom(qreal zoom)
{
    zoom = max(zoom, 0.01); // Otherwise we would divide by 0 in the next call
    if (this->zoom() != zoom) {
        qreal f = zoom / this->zoom() - 1.0;
        pixmapItem->setScale(zoom);
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
    bool differentX = (pixmapItem->x() != pos.x());
    bool differentY = (pixmapItem->y() != pos.y());
    if (differentX || differentY) {
        pixmapItem->setPos(pos);
        if (differentX)
            emit canvasFrame()->xChanged();
        if (differentY)
            emit canvasFrame()->yChanged();
        emit canvasFrame()->positionChanged();
    }
}
