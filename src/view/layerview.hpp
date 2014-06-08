#ifndef LAYERVIEW_HPP
#define LAYERVIEW_HPP


#include <QWidget>
#include <QGraphicsView>
#include <QList>
#include <QSize>
#include <QGraphicsObject>

#include <ApricotModel>

#include <QParallelAnimationGroup>
#include "graphicsanimationframeitem.hpp"
#include "graphicsemptyitem.hpp"

class LayerViewItem;

class LayerView : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(AnimationFrame *frame READ frame WRITE setFrame NOTIFY layersChanged)

public:
    explicit LayerView(QWidget *parent = 0);

    AnimationFrame *frame() const;
    void setFrame(AnimationFrame *frame);

signals:
    void layersChanged();

protected:
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *object, QEvent *event) override;

private:
    static const int SPACING_UNIT = 2;
    static const int ITEM_WIDTH = 150;
    static const int ITEM_HEIGHT = 150;
    static const int TRANSITION_DURATION = 150;

    QGraphicsView *graphicsView;
    QGraphicsScene *scene;
    QList<LayerViewItem *> items;
    GraphicsEmptyItem *dragItem; // To-Do
    int dropIndex;
    QParallelAnimationGroup draggedItemsAnimation;
    QParallelAnimationGroup itemsAnimation;

    AnimationFrame *m_frame;

    void setupScene();
    void updateSceneRect();

    void layOut();
    void layOutScene(bool animate);
    void layOutItems(bool animate);

    bool tryBeginDrag(QPointF dragBeginPos);
    void drag(QPointF dPos);
    void endDrag();

private slots:
    void onLayersChanged();
};

class LayerViewItem : public QGraphicsObject
{
    Q_OBJECT
    Q_PROPERTY(const Layer layer READ layer)
    Q_PROPERTY(const QSize &size READ size WRITE setSize)
    Q_PROPERTY(int width READ width)
    Q_PROPERTY(int height READ height)

public:
    LayerViewItem(const Layer *layer, QGraphicsItem *parent = nullptr);

    const Layer *layer() const;

    const QSize &size() const;
    void setSize(const QSize &size);
    void setSize(qreal width, qreal height);

    int width() const;
    int height() const;

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    static const int SPACING_UNIT = 6;

    QSize m_size;
    const Layer *m_layer;

};

inline AnimationFrame *LayerView::frame() const
{
    return m_frame;
}

inline const Layer *LayerViewItem::layer() const
{
    return m_layer;
}

inline const QSize &LayerViewItem::size() const
{
    return m_size;
}

inline void LayerViewItem::setSize(const QSize &size)
{
    m_size = size;
}

inline void LayerViewItem::setSize(qreal width, qreal height)
{
    setSize(QSize(width, height));
}

inline int LayerViewItem::width() const
{
    return size().width();
}

inline int LayerViewItem::height() const
{
    return size().height();
}



#endif // LAYERVIEW_HPP
