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
    enum AddButton {
        AddBeforeButton,
        AddAfterButton
    };

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
    AnimationViewItem *newItem(AnimationFrame *frame);
    void updateSceneRect();

    void layOut();
    void layOutScene(bool animate);
    void layOutItems(bool animate);

    bool tryBeginDrag(QPointF dragBeginPos);
    void drag(QPointF dPos);
    void endDrag();

    void handleAddButtonClick(AddButton button);

private slots:
    void onFramesChanged();

    friend class AnimationViewItem;
};


inline Project *AnimationView::project() const
{
    return m_project;
}

inline AnimationFrame *AnimationView::activeFrame() const
{
    return m_activeFrame;
}

#endif // VIEW_ANIMATIONVIEW_HPP
