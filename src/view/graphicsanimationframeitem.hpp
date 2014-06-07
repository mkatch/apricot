#ifndef VIEW_GRAPHICSANIMATIONFRAMEITEM_HPP
#define VIEW_GRAPHICSANIMATIONFRAMEITEM_HPP

#include <QGraphicsItem>

#include <apricotcore.hpp>

class GraphicsAnimationFrameItem : public QGraphicsItem
{
public:
    GraphicsAnimationFrameItem(const AnimationFrame *frame, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    const AnimationFrame *frame;
};

#endif // VIEW_GRAPHICSANIMATIONFRAMEITEM_HPP
