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
    canvas->fill(Qt::transparent);
    QPainter painter(canvas);
    painter.drawPixmap(0, 0, pixmap);
}
