#ifndef VIEW_ANIMATIONVIEW_HPP
#define VIEW_ANIMATIONVIEW_HPP

#include <QWidget>
#include <QGraphicsView>
#include <QList>
#include <QSize>
#include <QParallelAnimationGroup>

#include "apricotcore.hpp"

#include "graphicsanimationframeitem.hpp"
#include "graphicsemptyitem.hpp"

class AnimationViewItem;

class AnimationView : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(Project *project READ project WRITE setProject NOTIFY projectChanged)

public:
    explicit AnimationView(QWidget *parent = 0);

    Project *project() const { return m_project; }
    void setProject(Project *project);

signals:
    void projectChanged();

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

    void setupScene();

    void layOut();
    void layOutScene(bool animate);
    void layOutItems(bool animate);

    bool tryBeginDrag(QPointF dragBeginPos);
    void drag(QPointF dPos);
    void endDrag();
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

    const AnimationFrame *frame() const { return m_frame; }

    const QSize &size() const { return m_size; }
    void setSize(const QSize &size) { m_size = size; }
    void setSize(qreal width, qreal height) { setSize(QSize(width, height)); }

    int width() const { return m_size.width(); }
    int height() const { return m_size.height(); }

    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    static const int SPACING_UNIT = 6;

    QSize m_size;
    const AnimationFrame *m_frame;
};

#endif // VIEW_ANIMATIONVIEW_HPP
