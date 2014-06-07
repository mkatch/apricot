#include "qpainterextensions.hpp"

/*!
 * \class QPainterExtensions
 * \inmodule view
 *
 * \brief Provides additional drawing routines for QPainter
 *
 * This class is designed to achieve coherent visual appearance of some compound elements painted
 * with QPainter.
 */

/*!
 * \brief Constructs QPainteExtensions with underlying QPainter \a painter.
 */
QPainterExtensions::QPainterExtensions(QPainter *painter) :
    painter(painter)
{
    // Do nothing
}

/*!
 * \brief Draws a given \a frame into given \a targetRect.
 *
 * This is an overloaded method.
 */
void QPainterExtensions::drawAnimationFrame(const QRect &targetRect, const AnimationFrame *frame)
{
    foreach (Layer* layer, frame->layers())
        painter->drawPixmap(targetRect, layer->canvas()->pixmap());
}

/*!
 * \brief Draws a given \a frame at coordinates (\a x, \a y).
 *
 * This is an overloaded method.
 */
void QPainterExtensions::drawAnimationFrame(int x, int y, const AnimationFrame *frame)
{
    drawAnimationFrame(QRect(x, y, frame->width(), frame->height()), frame);
}

/*!
 * \brief Draws a given \a frame into rect defined by \a x, \a y, \a width, \a height.
 *
 * Thisis an overloaded method.
 */
void QPainterExtensions::drawAnimationFrame(
    int x, int y, int width, int height,
    const AnimationFrame *frame
)
{
    drawAnimationFrame(QRect(x, y, width, height), frame);
}
