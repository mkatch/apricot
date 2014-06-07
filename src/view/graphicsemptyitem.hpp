#ifndef VIEW_GRAPHICSEMPTYITEM_HPP
#define VIEW_GRAPHICSEMPTYITEM_HPP

#include <QGraphicsItem>

class GraphicsEmptyItem : public QGraphicsItem
{
public:
    GraphicsEmptyItem(QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void addChildItems(QList<QGraphicsItem *> childItems);
    void clearChildItems();
};

#endif // VIEW_GRAPHICSEMPTYITEM_HPP
