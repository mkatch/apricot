#include "animationframeview.hpp"

#include <algorithm>
using std::max;

#include <ApricotUtils>

#include "graphicsanimationframeitem.hpp"

/*!
 * \class AnimationFrameView
 * \inmodule view
 *
 * \brief A widget for displaying a single animation frame.
 *
 * The displayed frame may be dragged around and zoomed in and out programmatically.
 */

// Properties

/*!
 * \property AnimationFrameView::frame
 * \brief The displayed AnimationFrame.
 */

/*!
 * \property AnimationFrameView::scale
 * \brief The zoom factor.
 */

/*!
 * \property AnimationFrameView::translation
 * \brief The display offset.
 *
 * \sa translate()
 */

/*!
 * \property AnimationFrameView::transform
 * \brief Tha transformation for transition frame coordinate system to the view coordinates.
 */

// Methods

/*!
 * \brief Constructs the view with parent widget \a parent.
 */
AnimationFrameView::AnimationFrameView(QWidget *parent) :
    QWidget(parent),
    graphicsView(new QGraphicsView(this)),
    scene(new QGraphicsScene(this)),
    placeholderPixmap(256, 256),
    m_frame(nullptr)
{
    graphicsView->setScene(scene);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    frameItem = scene->addPixmap(placeholderPixmap);
    layOut();
}

void AnimationFrameView::setFrame(const AnimationFrame *frame)
{
    if (m_frame == frame)
        return;

    m_frame = frame;

    QPointF pos = frameItem->pos();
    qreal scale = frameItem->scale();
    scene->removeItem(frameItem);
    delete frameItem;

    frameItem = (m_frame != nullptr)
        ? static_cast<QGraphicsItem *>(new GraphicsAnimationFrameItem(frame))
        : static_cast<QGraphicsItem *>(new QGraphicsPixmapItem(placeholderPixmap));
    frameItem->setPos(pos);
    frameItem->setScale(scale);
    scene->addItem(frameItem);

    emit frameChanged();
}

void AnimationFrameView::setScale(qreal scale)
{
    if (frameItem->scale() == scale)
        return;

    frameItem->setScale(scale);
    emit scaleChanged();
    emit transformChanged();
}

/*!
 * \fn AnimationFrameView::scale(qreal scale)
 * \brief Multiply the current scale by \a scale.
 */

void AnimationFrameView::setTranslation(const QPointF &translation)
{
    if (frameItem->pos() == translation)
        return;

    frameItem->setPos(translation);
    emit translationChanged();
    emit transformChanged();
}

/*!
 * \fn AnimationFrameView::translate(const QPointF &translation)
 * \brief Adds \a translation to current translation.
 */

/*!
 * \fn AnimationFrameView::translate(qreal x, qreal y)
 * \brief Adds (\a x, \a y) to current translation.
 */

QTransform AnimationFrameView::transform() const
{
    return QTransform().scale(scale(), scale()).translate(translation().x(), translation().y());
}

/*!
 * \brief Maps \a point from view coordinates to frame coordinates.
 *
 * \sa mapFromFrame()
 */
QPointF AnimationFrameView::mapToFrame(const QPointF &point) const
{
    return (point - translation()) / scale();
}

/*!
 * \brief Maps \a point from frame coordinates to view coordinates.
 *
 * \sa mapToFrame()
 */
QPointF AnimationFrameView::mapFromFrame(const QPointF &point) const
{
    return point * scale() + translation();
}

/*!
 * \brief Called when the widget is resized.
 *
 * The \a event contains the information about the resize event.
 */
void AnimationFrameView::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    layOut();
}

/*!
 * \brief Lays out child widgets.
 *
 * Called after resize and at creation.
 */
void AnimationFrameView::layOut()
{
    graphicsView->resize(size());
    graphicsView->setSceneRect(2, 2, width() - 2, height() - 2);
}
