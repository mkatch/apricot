#include "CanvasProvider.hpp"

#include <QDebug>
#include <QBitmap>

#include "Canvas.hpp"

CanvasProvider::CanvasProvider(const Canvas *canvas) :
    QQuickImageProvider(Pixmap),
    m_canvas(canvas)
{
    // Do nothing
}

void CanvasProvider::setCanvas(const Canvas *canvas)
{
    m_canvas = canvas;
}

QPixmap CanvasProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    if (id != "canvas")
        return QPixmap();

    QPixmap pixmap = (m_canvas != nullptr) ? m_canvas->toPixmap() : QBitmap(1, 1);
    if (size != nullptr)
        *size = pixmap.size();
    return requestedSize.isValid() ? pixmap.scaled(requestedSize) : pixmap;
}
