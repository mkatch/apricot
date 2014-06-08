#include "graphicsemptyitem.hpp"

/*!
 * \class GraphicsEmptyItem
 * \inmodule view
 *
 * \brief A QGraphicsItem with no visual representation
 *
 * Used to provide a logical parent for other QGraphicsItem objects. therefore it contains some
 * convenient parent-child manipulation methods.
 */

/*!
 * \brief Constructs the item with parent item \a parent.
 */
GraphicsEmptyItem::GraphicsEmptyItem(QGraphicsItem *parent) :
    QGraphicsItem(parent)
{
    // Do nothing
}

/*!
 * \brief Returns the bounding rectangle of the item.
 */
QRectF GraphicsEmptyItem::boundingRect() const
{
    return QRectF(0, 0, 0, 0);
}

/*!
 * \brief Paints the item using \a painter.
 *
 * Called by QGraphicsView when it's contents need to be updated. The \a option parameter provides
 * style option and \a widget is optional and points to widget that is being painted if provided.
 *
 * This QGraphicsItem has no contents so this method does nothing.
 */
void GraphicsEmptyItem::paint(
    QPainter *painter,
    const QStyleOptionGraphicsItem *option,
    QWidget *widget
)
{
    Q_UNUSED(painter)
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

/*!
 * \brief Sets the parent item of all \a childItems to \e this.
 */
void GraphicsEmptyItem::addChildItems(QList<QGraphicsItem *> childItems)
{
    foreach (QGraphicsItem *item, childItems)
        item->setParentItem(this);
}

/*!
 * \brief Sets the parent item of all child items to the parent of the GraphicsEmptyItem.
 */
void GraphicsEmptyItem::clearChildItems()
{
    foreach (QGraphicsItem *item, childItems())
        item->setParentItem(this->parentItem());
}
