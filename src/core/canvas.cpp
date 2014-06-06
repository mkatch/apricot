#include "canvas.hpp"

Canvas::Canvas()
{
    // Do nothing.
}

Canvas::Canvas(int width, int height) :
    m_pixmap(width, height)
{
    // Do nothing.
}

Canvas::Canvas(const QSize &size) :
    m_pixmap(size)
{
    // Do nothing.
}

Canvas::Canvas(const Canvas &canvas) :
    m_pixmap(canvas.m_pixmap)
{
    // Do nothing.
}

Canvas::Canvas(const QPixmap &pixmap) :
    m_pixmap(pixmap)
{
    // Do nothing.
}

void Canvas::fill(const QColor &color)
{
    m_pixmap.fill(color);
}

void Canvas::setSize(int width, int height)
{
    setSize(QSize(width, height));
}

void Canvas::setSize(const QSize &size)
{
    // No need to create whole new pixmap if the size doesn't actually change.
    if(size == this->size())
        return;

    // Simply create larger canvas and paint current pixmap on it. Slow but works.
    QPixmap resized(size);
    resized.fill(Qt::transparent);

    QPainter painter(&resized);
    painter.drawPixmap(0, 0, m_pixmap);

    m_pixmap.swap(resized);
}
