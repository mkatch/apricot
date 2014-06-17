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
 * \fn const QBrush &Painter::brush() const
 * \brief Returns the painter's current brush.
 */

/*!
 * \fn const QPen &Painter::pen() const
 * \brief Returns the painter's current pen.
 */

/*!
 * \fn QPainter::CompositionMode Painter::compositionMode() const
 * \brief Returns the painter's composition mode.
 */

/*!
 * \fn void Painter::setBrush(const QBrush &brush)
 * \brief Sets the painter's brush to the given \a brush.
 */

/*!
 * \fn void Painter::setPen(const QPen &pen)
 * \brief Sets the painter's pen to the given \a pen.
 */

/*!
 * \fn void Painter::setCompositionMode(QPainter::CompositionMode mode)
 * \brief Sets the painter's composition mode to \a mode.
 */

/*!
 * \fn const QRect &Painter::boundingBox() const
 * \brief Returns bounding box of area changed by the drawing events.
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
 * \brief Draws an image given as \a pixmap at point (0, 0).
 */
void Painter::drawImage(QPixmap pixmap)
{
    painter.drawPixmap(0, 0, pixmap);
    m_boundingBox |= pixmap.rect();
}

/*!
 * \brief Draws an image loaded from \a fileName at point (0, 0).
 *
 * This is a temporal method for test purposes in early stages of development.
 *
 * \overload
 */
void Painter::drawImage(QString fileName)
{
    drawImage(QPixmap(fileName));
}

/*!
 * \brief Draws a point at \a p using the current pen's color and width.
 */
void Painter::drawPoint(const QPoint &p)
{
    int size = painter.pen().width();
    QRect rect(p.x() - size / 2, p.y() - size / 2, size, size);

    // We use this instead of QPainter::drawPoint to avoid weird problems.
    painter.fillRect(rect, painter.pen().color());
    m_boundingBox |= rect;
}

/*!
 * \fn void Painter::drawPoint(int x, int y)
 * \brief Draw a point at (\a x, \a y) using the current pen's color and width.
 *
 * \overload
 */

/*!
 * \brief Draws a line from \a p1 to \a p2.
 */
void Painter::drawLine(const QPoint &p1, const QPoint &p2)
{
    QRect rect;
    rect.setTopLeft(p1);
    rect.setBottomRight(p2);
    rect = rect.normalized();

    painter.drawLine(p1, p2);

    int delta = painter.pen().width() % 2;
    m_boundingBox |= rect.adjusted(-delta - 1, -delta - 1, +delta, +delta);
}

/*!
 * \fn void Painter::drawLine(int x1, int y1, int x2, int y2)
 * \brief Draws a line from (\a x1, \a y1) to (\a x2, \a y2).
 *
 * \overload
 */

/*!
 * \brief Draws a rectangle defined by \a rect with the current pen and brush.
 */
void Painter::drawRect(const QRect &rect)
{
    painter.drawRect(rect);

    int delta = pen().width() / 2;
    int c = pen().width() % 2;
    m_boundingBox |= rect.adjusted(-delta, -delta, +delta + c, +delta + c);
}

/*!
 * \fn void Painter::drawRect(int x, int y, int width, int height)
 * \brief Draws a rectangle beginning at (\a x, \a y) of given \a width and \a height.
 *
 * \overload
 */

/*!
 * \brief Draws the ellipse defined by \a rect with the current pen and brush.
 */
void Painter::drawEllipse(const QRect &rect)
{
    painter.drawEllipse(rect);

    int delta = pen().width() / 2;
    int c = pen().width() % 2;
    m_boundingBox |= rect.adjusted(-delta, -delta, +delta + c, +delta + c);
}

/*!
 * \fn void Painter::drawEllipse(const QPoint &center, int rx, int ry)
 * \brief Draws the ellipse positioned at \a center with radii \a rx and \a ry.
 *
 * \overload
 */

/*!
 * \brief Draws a fragment of \a canvas at point \a p.
 *
 * The drawn fragment is determined by \a sourceRect.
 */
void Painter::drawCanvas(const QPoint &p, const Canvas &canvas, const QRect &sourceRect)
{
    painter.drawPixmap(p, canvas.pixmap(), sourceRect);
    m_boundingBox |= QRect(p, sourceRect.size());
}
