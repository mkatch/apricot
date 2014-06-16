#ifndef VIEW_ANIMATIONVIEW_HPP
#define VIEW_ANIMATIONVIEW_HPP

#include <QWidget>
#include <QGraphicsView>
#include <QList>
#include <QSize>
#include <QParallelAnimationGroup>

#include <ApricotModel>

#include "graphicsanimationframeitem.hpp"
#include "graphicsemptyitem.hpp"

class AnimationViewItem;

class AnimationView : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(Project *project READ project WRITE setProject)
    Q_PROPERTY(
        AnimationFrame *activeFrame
        READ activeFrame
        WRITE setActiveFrame
        NOTIFY activeFrameChanged
    )

public:
    explicit AnimationView(QWidget *parent = 0);

    Project *project() const;
    AnimationFrame *activeFrame() const;

public slots:
    void setProject(Project *project);
    void setActiveFrame(const AnimationFrame *frame);

signals:
    void activeFrameChanged(AnimationFrame *newFrame);

protected:
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *object, QEvent *event) override;

private:
    static const int SPACING_UNIT = 2;
    static const int ITEM_WIDTH = 150;
    static const int TRANSITION_DURATION = 150;

    QGraphicsView *graphicsView;
    QGraphicsScene *scene;
    QList<AnimationViewItem *> items;
    GraphicsEmptyItem *dragItem;
    int dropIndex;
    QParallelAnimationGroup draggedItemsAnimation;
    QParallelAnimationGroup itemsAnimation;

    Project *m_project;
    AnimationFrame *m_activeFrame;

    void setupScene();
    void updateSceneRect();

    void layOut();
    void layOutScene(bool animate);
    void layOutItems(bool animate);

    bool tryBeginDrag(QPointF dragBeginPos);
    void drag(QPointF dPos);
    void endDrag();

private slots:
    void onFramesChanged();
};

class AnimationViewItem : public QGraphicsObject
{
    Q_OBJECT
    Q_PROPERTY(const AnimationFrame *frame READ frame)
    Q_PROPERTY(const QSize &size READ size WRITE setSize)
    Q_PROPERTY(int width READ width)
    Q_PROPERTY(int height READ height)

public:
    AnimationViewItem(const AnimationFrame *frame, QGraphicsItem *parent = nullptr);

    const AnimationFrame *frame() const;

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
    const AnimationFrame *m_frame;
};

inline Project *AnimationView::project() const
{
    return m_project;
}

inline AnimationFrame *AnimationView::activeFrame() const
{
    return m_activeFrame;
}

inline const AnimationFrame *AnimationViewItem::frame() const
{
    return m_frame;
}

inline const QSize &AnimationViewItem::size() const
{
    return m_size;
}

inline void AnimationViewItem::setSize(const QSize &size)
{
    m_size = size;
}

inline void AnimationViewItem::setSize(qreal width, qreal height)
{
    setSize(QSize(width, height));
}

inline int AnimationViewItem::width() const
{
    return size().width();
}

inline int AnimationViewItem::height() const
{
    return size().height();
}

#endif // VIEW_ANIMATIONVIEW_HPP
