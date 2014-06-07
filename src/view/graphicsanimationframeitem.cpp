#include "graphicsanimationframeitem.hpp"

#include <QPainter>

#include "qpainterextensions.hpp"

/*!
 * \class GraphicsAnimationFrameItem
 * \inmodule view
 *
 * \brief A QGraphicsItem to display AnimationFrame.
 */

/*!
 * \brief Contructor.
 *
 * Creates an item that displays \a frame and sets \a parent as the parent item.
 */
GraphicsAnimationFrameItem::GraphicsAnimationFrameItem(
    const AnimationFrame *frame,
    QGraphicsItem *parent
) :
    QGraphicsItem(parent),
    frame(frame)
{
    // Do nothing
}

/*!
 * \brief Returns the bounding rectangle.
 */
QRectF GraphicsAnimationFrameItem::boundingRect() const
{
    return QRectF(QPointF(0, 0), frame->size());
}

/*!
 * \brief Paints the item using \a painter.
 *
 * Called by QGraphicsView when it's contents need to be updated. The \a option parameter provides
 * style option and \a widget is optional and points to widget that is being painted if provided.
 */
void GraphicsAnimationFrameItem::paint(
    QPainter *painter,
    const QStyleOptionGraphicsItem *option,
    QWidget *widget
)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    QPainterExtensions(painter).drawAnimationFrame(0, 0, frame);
}
