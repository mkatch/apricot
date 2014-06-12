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

/*!
 * \brief Draws checkered background behind a given \a layer.
 * \overload
 */
void QPainterExtensions::drawBackground(const Layer *layer)
{
    int off_white = 4294966265;
    int light_grey = 4291611852;
    QPixmap bg_pix(16, 16);
    bg_pix.fill(off_white);

    QPainter *p = new QPainter(&bg_pix);
    QBrush pnt_brush(light_grey);

    p->setBrush(pnt_brush);
    p->setPen(Qt::NoPen);

    p->drawRect(0, 0, 8, 8);
    p->drawRect(8, 8, 8,8);

    p->end();
    delete p;
    QBrush *brush = new QBrush(bg_pix);
    brush->setStyle(Qt::TexturePattern);

    painter->drawTiledPixmap(QRect(0, 0, layer->width(), layer->height()),bg_pix);
}

/*!
 * \brief Draws checkered background into given \a targetRect.
 * \overload
 */
void QPainterExtensions::drawBackground(const QRect &targetRect)
{
    int off_white = 4294966265;
    int light_grey = 4291611852;
    QPixmap bg_pix(16, 16);
    bg_pix.fill(off_white);

    QPainter *p = new QPainter(&bg_pix);
    QBrush pnt_brush(light_grey);

    p->setBrush(pnt_brush);
    p->setPen(Qt::NoPen);

    p->drawRect(0, 0, 8, 8);
    p->drawRect(8, 8, 8,8);

    p->end();
    delete p;
    painter->drawTiledPixmap(targetRect, bg_pix);
}

/*!
 * \brief Draws checkered background behind a given \a layer at coordinates (\a x, \a y).
 * \overload
 */
void QPainterExtensions::drawBackground(int x, int y, const Layer *layer)
{
    drawBackground(QRect(x, y, layer->width(), layer->height()));
}

/*!
 * \brief Draws checkered background into rect defined by \a x, \a y, \a width, \a height.
 * \overload
 */
void QPainterExtensions::drawBackground(
    int x, int y, int width, int height
)
{
    drawBackground(QRect(x, y, width, height));
}

/*!
 * \brief Return checkered \c QBrush made of given colors \a first and \a second.
 * \overload
 */
QBrush QPainterExtensions::getBrushForBackground(QColor first, QColor second)
{
    QPixmap bg_pix(16, 16);
    bg_pix.fill(first);

    QPainter *p = new QPainter(&bg_pix);
    QBrush pnt_brush(second);

    p->setBrush(pnt_brush);
    p->setPen(Qt::NoPen);

    p->drawRect(0, 0, 8, 8);
    p->drawRect(8, 8, 8,8);

    p->end();
    delete p;
    return QBrush(bg_pix);
}

/*!
 * \brief Return checkered \c QBrush made of given colors \a first and \a second.
 * \overload
 */
QBrush QPainterExtensions::getBrushForBackground(int first, int second)
{
    return getBrushForBackground(QColor(first), QColor(second));
}

/*!
 * \brief Return checkered \c QBrush made of white and light gray.
 * \overload
 */
QBrush QPainterExtensions::getBrushForBackground()
{
    return getBrushForBackground(4294966265, 4291611852);
}


