#include "painter.hpp"

#include <QPainter>

#include "canvas.hpp"

Painter::Painter(Canvas &canvas) :
    painter(new QPainter(&canvas.m_pixmap))
{
    // Do nothing
}

void Painter::drawImage(QString fileName)
{
    painter->drawPixmap(0, 0, QPixmap(fileName));
}
