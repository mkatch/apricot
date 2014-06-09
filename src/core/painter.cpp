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
 * \fn const QRect &Painter::boundingBox() const
 * \brief Returns bounding box of area changed by the drawing events.
 */

/*!
 * \fn const QColor &Painter::brushColor() const
 * \brief Returns current brush color.
 */

/*!
 * \fn int Painter::brushSize() const
 * \brief Returns current brush size.
 */

/*!
 * \fn void Painter::setBrushColor(const QColor &color)
 * \brief Changes current brush color to \a color.
 */

/*!
 * \fn void Painter::setBrushSize(int size)
 * \brief Changes current brush size to \a size.
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
    QPixmap pixmap(fileName);

    painter->drawPixmap(0, 0, pixmap);
    m_boundingBox |= pixmap.rect();
}

/*!
 * \brief Draws a point centered around \a x and \a y coordinates.
 */
void Painter::drawPoint(int x, int y)
{
    int size = brushSize();
    QRect drawingRect(x - size / 2, y - size / 2, size, size);

    // We use this instead of QPainter::drawPoint to avoid weird problems.
    painter->fillRect(drawingRect, brushColor());
    m_boundingBox |= drawingRect;
}

/*!
 * \fn void Painter::drawPoint(const QPoint &position)
 * \brief Draws a point centered around \a position.
 *
 * This is an overloaded function.
 */
