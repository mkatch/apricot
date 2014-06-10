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
 * \overload
 */
void QPainterExtensions::drawAnimationFrame(const QRect &targetRect, const AnimationFrame *frame)
{
    foreach (const Layer* layer, frame->layers())
        painter->drawPixmap(targetRect, layer->canvas().pixmap());
}

/*!
 * \brief Draws a given \a frame at coordinates (\a x, \a y).
 * \overload
 */
void QPainterExtensions::drawAnimationFrame(int x, int y, const AnimationFrame *frame)
{
    drawAnimationFrame(QRect(x, y, frame->width(), frame->height()), frame);
}

/*!
 * \brief Draws a given \a frame into rect defined by \a x, \a y, \a width, \a height.
 * \overload
 */
void QPainterExtensions::drawAnimationFrame(
    int x, int y, int width, int height,
    const AnimationFrame *frame
)
{
    drawAnimationFrame(QRect(x, y, width, height), frame);
}

/*!
 * \brief Draws a given \a layer into given \a targetRect.
 * \overload
 */
void QPainterExtensions::drawLayer(const QRect &targetRect, const Layer *layer)
{
    painter->drawPixmap(targetRect, layer->canvas().pixmap());
}

/*!
 * \brief Draws a given \a layer at coordinates (\a x, \a y).
 * \overload
 */
void QPainterExtensions::drawLayer(int x, int y, const Layer *layer)
{
    drawLayer(QRect(x, y, layer->width(), layer->height()), layer);
}

/*!
 * \brief Draws a given \a layer into rect defined by \a x, \a y, \a width, \a height.
 * \overload
 */
void QPainterExtensions::drawLayer(
    int x, int y, int width, int height,
    const Layer *layer
)
{
    drawLayer(QRect(x, y, width, height), layer);
}
