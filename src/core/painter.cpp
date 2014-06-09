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
    painter(&canvas.m_pixmap)
{
    painter.setRenderHint(QPainter::NonCosmeticDefaultPen);
}

/*!
 * \brief Draws an image loaded from \a fileName at point (0, 0).
 *
 * This is a temporal method for test purposes in early stages of development.
 */
void Painter::drawImage(QString fileName)
{
    QPixmap pixmap(fileName);

    painter.drawPixmap(0, 0, pixmap);
    m_boundingBox |= pixmap.rect();
}

void Painter::drawPoint(const QPoint &p)
{
    int size = painter.pen().width();
    QRect rect(p.x() - size / 2, p.y() - size / 2, size, size);

    // We use this instead of QPainter::drawPoint to avoid weird problems.
    painter.fillRect(rect, painter.pen().color());
    m_boundingBox |= rect;
}

void Painter::drawLine(const QPoint &p1, const QPoint &p2)
{
    QRect rect;
    rect.setTopLeft(p1);
    rect.setBottomRight(p2);
    rect = rect.normalized();

    painter.drawLine(p1, p2);
    m_boundingBox |= rect;
}

void Painter::drawRect(const QRect &rect)
{
    painter.drawRect(rect);
    m_boundingBox |= rect;
}

void Painter::drawEllipse(const QRect &rect)
{
    painter.drawEllipse(rect);
    m_boundingBox |= rect;
}
