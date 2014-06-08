#include "painter.hpp"

#include <QPainter>

#include "canvas.hpp"

/*!
 * \class Painter
 * \inmodule core
 *
 * \brief Allows drawing on Canvas.
 */

/*!
 * \brief Constructs a painter that paints on \a canvas.
 */
Painter::Painter(Canvas &canvas) :
    painter(new QPainter(&canvas.m_pixmap))
{
    // Do nothing
}

/*!
 * \brief Draws an image loaded from \a fileName at point (0, 0).
 *
 * This is a temporal method for test purposes in early stages of development.
 */
void Painter::drawImage(QString fileName)
{
    painter->drawPixmap(0, 0, QPixmap(fileName));
}
