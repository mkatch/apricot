#include "animationframeview.hpp"

#include <algorithm>
using std::max;

#include <ApricotUtils>

#include "graphicsanimationframeitem.hpp"
#include "tool.hpp"
#include "toolevents.hpp"

/*!
 * \class AnimationFrameView
 * \inmodule view
 *
 * \brief A widget for displaying a single animation frame.
 *
 * The displayed frame may be dragged around and zoomed in and out programmatically. User can
 * perform actions on this view and the underlying frame using tools. A tool is any specialization
 * of Tool class. Once a tool is attached to AnimationFrameView, it receives mouse and keyboard
 * events.
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

/*!
 * \property AnimationFrameView::tool
 * \brief The active tool.
 *
 * This property may be \c nullptr meaning that the tool is not set.
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

/*!
 * \fn AnimationFrameView::~AnimationFrameView()
 * \brief Destroys the view.
 */

void AnimationFrameView::setFrame(AnimationFrame *frame)
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

void AnimationFrameView::setTool(Tool *tool)
{
    if (m_tool == tool)
        return;

    if (m_tool != nullptr)
        m_tool->setView(nullptr);
    m_tool = tool;
    if (m_tool != nullptr)
        m_tool->setView(this);
    emit toolChanged();
    update();
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
 * \brief Handles mouse press event \a event.
 *
 * This results in invoking Tool::mousePressEvent() of the active tool.
 */
void AnimationFrameView::mousePressEvent(QMouseEvent *event)
{
    if (tool() == nullptr)
        return event->ignore();

    ToolMouseEvent toolEvent(
        event,
        mapToFrame(event->localPos()),
        event->localPos(),
        event->button(),
        event->buttons(),
        event->modifiers()
    );
    tool()->mousePressEvent(&toolEvent);
    lastMousePos = event->localPos();
}

/*!
 * \brief Handles mouse release event \a event.
 *
 * This results in invoking Tool::mouseReleaseEvent() of the active tool.
 */
void AnimationFrameView::mouseReleaseEvent(QMouseEvent *event)
{
    if (tool() == nullptr)
        return event->ignore();

    ToolMouseEvent toolEvent(
        event,
        mapToFrame(event->localPos()),
        event->localPos(),
        event->button(),
        event->buttons(),
        event->modifiers()
    );
    tool()->mouseReleaseEvent(&toolEvent);
}

/*!
 * \brief Handles double click event \a event.
 *
 * This results in invoking Tool::mouseDoubleClickEvent() of the active tool.
 */
void AnimationFrameView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (tool() == nullptr)
        return event->ignore();

    ToolMouseEvent toolEvent(
        event,
        mapToFrame(event->localPos()),
        event->localPos(),
        event->button(),
        event->buttons(),
        event->modifiers()
    );
    tool()->mouseDoubleClickEvent(&toolEvent);
}

/*!
 * \brief Handles mouse move event \a event.
 *
 * This results in invoking Tool::mouseMoveEvent() of the active tool.
 */
void AnimationFrameView::mouseMoveEvent(QMouseEvent *event)
{
    if (tool() == nullptr)
        return event->ignore();

    ToolMouseMoveEvent toolEvent(
        event,
        mapToFrame(event->localPos()),
        mapToFrame(lastMousePos),
        event->localPos(),
        lastMousePos,
        event->buttons(),
        event->modifiers()
    );
    tool()->mouseMoveEvent(&toolEvent);
    lastMousePos = event->localPos();
}

/*!
 * \brief Handles mouse wheel event \a event.
 *
 * This results in invoking Tool::wheelEvent() of the active tool.
 */
void AnimationFrameView::wheelEvent(QWheelEvent *event)
{
    if (tool() == nullptr)
        return event->ignore();

    ToolWheelEvent toolEvent(
        event,
        mapToFrame(event->posF()),
        event->posF(),
        event->pixelDelta(),
        event->angleDelta(),
        event->buttons(),
        event->modifiers()
    );
    tool()->wheelEvent(&toolEvent);
}

/*!
 * \brief Handles key press event \a event.
 *
 * This results in invoking Tool::keyPressEvent() of the active tool.
 */
void AnimationFrameView::keyPressEvent(QKeyEvent *event)
{
    if (tool() == nullptr)
        return event->ignore();

    ToolKeyEvent toolEvent(
        event,
        event->key(),
        event->modifiers()
    );
    tool()->keyPressEvent(&toolEvent);
}

/*!
 * \brief Handles key release event \a event.
 *
 * This results in invoking Tool::keyReleaseEvent() of the active tool.
 */
void AnimationFrameView::keyReleaseEvent(QKeyEvent *event)
{
    if (tool() == nullptr)
        return event->ignore();

    ToolKeyEvent toolEvent(
        event,
        event->key(),
        event->modifiers()
    );
    tool()->keyReleaseEvent(&toolEvent);
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
