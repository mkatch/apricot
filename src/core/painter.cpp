#include "painter.hpp"

#include <QPainter>

#include "Canvas.hpp"

Painter::Painter(Canvas *canvas) :
    canvas(canvas)
{
    // Do nothing
}

Painter::Painter(const Painter &other) :
    canvas(other.canvas)
{
    // Do nothing
}

void Painter::load(QString fileName)
{
    QPixmap pixmap(fileName);
    QPainter painter(canvas);
    canvas->fill(Qt::transparent);
    painter.drawPixmap(0, 0, pixmap);
}
