#include "animationview.hpp"

#include <algorithm>
#include <QGraphicsSceneEvent>
#include <QPropertyAnimation>

#include <ApricotUtils>
#include <ApricotCore>

#include "qpainterextensions.hpp"

using std::sort;

/*!
 * \class AnimationView
 * \inmodule view
 *
 * \brief A widget to manage animation frames
 */

 // Properties

/*!
 * \property AnimationView::project
 * \brief The project the animation frames of which are displayed.
 */

/*!
 * \property AnimationView::activeFrame
 * \brief The active frame.
 */

// Methods

/*!
 * \brief Constructs AnimationView with parent widget \a parent.
 */
AnimationView::AnimationView(QWidget *parent) :
    QWidget(parent),
    onionSkinPrevious(1),
    onionSkinNext(1),
    graphicsView(new QGraphicsView(this)),
    scene(new QGraphicsScene(this)),
    dragItem(nullptr),
    dropIndex(-1),
    m_project(nullptr),
    m_activeFrame(nullptr)
{
    setMinimumHeight(150);
    graphicsView->setRenderHint(QPainter::Antialiasing);
    graphicsView->setScene(scene);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scene->installEventFilter(this);
    layOut();
}

void AnimationView::setProject(Project *project)
{
    if (m_project == project)
        return;

    m_project = project;
    connect(project, SIGNAL(framesChanged()), this, SLOT(onFramesChanged()));
    setupScene();
    updateSceneRect();
    setActiveFrame(project->frame(0));
}

void AnimationView::setActiveFrame(const AnimationFrame *frame)
{
    if (m_activeFrame == frame)
        return;

    if (frame->project() != project()) {
        qWarning(
            "AnimationView::setActiveFrame(): A frame that is not a member of displayed "
            "project was tried to be set as active frame."
        );
        return;
    }

     // This is safe cause the view has acces to all frames anyway.
    m_activeFrame = const_cast<AnimationFrame *>(frame);
    emit activeFrameChanged(m_activeFrame);
}

/*!
 * \brief Occurs when the widget is resized.
 *
 * Event information are provided in \a event.
 */
void AnimationView::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    layOut();
}

/*!
 * \brief Filters events if this view has been installed as an event filter for \a object.
 *
 * This is an overiden method. AnimationView is installed as an event filter for it's underlying
 * QGraphicsScene. This is done to handle dragging and does not reject any \a event. This
 * implementation always returns \c false which means all events are passed further.
 */
bool AnimationView::eventFilter(QObject *object, QEvent *event)
{
    if (object == scene) {
        switch (event->type()) {
        case QEvent::GraphicsSceneMouseMove: {
            QGraphicsSceneMouseEvent *mouse = static_cast<QGraphicsSceneMouseEvent *>(event);
            if (mouse->buttons() != Qt::LeftButton)
                break;
            if (dragItem == nullptr && !tryBeginDrag(mouse->buttonDownScenePos(Qt::LeftButton)))
                break;
            drag(mouse->scenePos() - mouse->lastScenePos());
            break;
        }
        case QEvent::GraphicsSceneMouseRelease: {
            QGraphicsSceneMouseEvent *mouse = static_cast<QGraphicsSceneMouseEvent *>(event);
            if (mouse->button() == Qt::LeftButton && dragItem != nullptr){
                endDrag();

            }
            break;
        }
        default: break;
        }
    } else { // object must be one of items
        AnimationViewItem *item = static_cast<AnimationViewItem *>(object);
        if (event->type() == QEvent::GraphicsSceneMousePress) {
            QGraphicsSceneMouseEvent *mouse = static_cast<QGraphicsSceneMouseEvent *>(event);
            if (mouse->button() == Qt::LeftButton)
                setActiveFrame(item->frame());
        }
    }

    return false;
}

/*!
 * \brief Creates the scene.
 */
void AnimationView::setupScene()
{
    // Remove current thumbnails
    for (int i = 0; i < items.count(); ++i) {
        scene->removeItem(items[i]);
        delete items[i];
    }
    items.clear();

    if (project() == nullptr)
        return;

    // Add new thumbnails
    for (int i = 0; i < project()->frameCount(); ++i) {
        AnimationViewItem *item = new AnimationViewItem(project()->frame(i));
        item->setFlag(QGraphicsItem::ItemIsSelectable);
        item->installEventFilter(this);
        scene->addItem(item);
        items.append(item);
    }

    layOutScene(false);
}

/*!
 * \brief Sets the scene rect of the QGraphicsView to tigtly fit contents.
 */
void AnimationView::updateSceneRect()
{
    graphicsView->setSceneRect(
        0, 0,
        (items.count() + 1) * SPACING_UNIT + items.count() * ITEM_WIDTH,
        height()
    );
}

/*! \brief Lays out the child widgets and graphics scene.
 *
 * Called at construction time and after resize.
 */
void AnimationView::layOut()
{
    graphicsView->setGeometry(0, 0, width(), height());
    layOutScene(false);
    updateSceneRect();
}

/*!
 * \brief Lays out the scene items.
 *
 * Called in response to resizing. If \a animate is \c true, item transitions are
 * animated. Otherwise the change is immediate.
 */
void AnimationView::layOutScene(bool animate)
{
    if (project() == nullptr)
        return;

    foreach (AnimationViewItem *item, items)
        item->setSize(ITEM_WIDTH, max(height() - 2 * SPACING_UNIT, 0));

    layOutItems(animate);
}

/*!
 * \brief Lays out scene items that represent the animation frames.
 *
 * Called in response to rezising and dragging. If \a animate is \c true, item transitions are
 * animated. Otherwise the change is immediate.
 */
void AnimationView::layOutItems(bool animate)
{
    itemsAnimation.clear();

    int draggedItemCount = (dragItem != nullptr) ? dragItem->childItems().count() : 0;
    int laidOutItemCount = 0;
    foreach (AnimationViewItem *item, items)
        if (dragItem == nullptr || item->parentItem() != dragItem) {
            qreal itemX = SPACING_UNIT + laidOutItemCount * (ITEM_WIDTH + SPACING_UNIT);
            if (dropIndex <= laidOutItemCount)
                itemX += draggedItemCount * (SPACING_UNIT + ITEM_WIDTH);
            if (animate) {
                QPropertyAnimation *animation = new QPropertyAnimation(item, "pos");
                animation->setDuration(TRANSITION_DURATION);
                animation->setEndValue(QPointF(itemX, SPACING_UNIT));
                animation->setEasingCurve(QEasingCurve::InOutQuad);
                itemsAnimation.addAnimation(animation);
            } else {
                item->setX(itemX);
            }
            ++laidOutItemCount;
        }

    if (itemsAnimation.animationCount() > 0)
        itemsAnimation.start();
}

bool compareItemsByX(QGraphicsItem *i, QGraphicsItem *j)
{
    return i->x() < j->x();
}

/*!
 * \brief Initiates dragging.
 *
 * The mouse position at the moment of pressing is passed in \a dragBeginPos. The method returns
 * \c false if the conditions for dragging are not fulfilled.
 */
bool AnimationView::tryBeginDrag(QPointF dragBeginPos)
{
    QGraphicsItem *grabbedItem = scene->itemAt(dragBeginPos, graphicsView->transform());
    if (grabbedItem == NULL || !grabbedItem->isSelected())
        return false;
    // Only frame items are selectable so if we reached this, it means the user grabbed a frame

    QList<QGraphicsItem *> draggedItems = scene->selectedItems();
    sort(draggedItems.begin(), draggedItems.end(), compareItemsByX);
    dragItem = new GraphicsEmptyItem;
    scene->addItem(dragItem);
    dragItem->addChildItems(draggedItems);
    dragItem->setZValue(1);

    int grabbedItemIndex = draggedItems.indexOf(grabbedItem);
    qreal firstDraggedItemX = grabbedItem->x() - grabbedItemIndex * (SPACING_UNIT + ITEM_WIDTH);
    for (int i = 0; i < draggedItems.count(); ++i) {
        AnimationViewItem *item = static_cast<AnimationViewItem *>(draggedItems[i]);
        QPropertyAnimation *animation = new QPropertyAnimation(item, "x");
        animation->setDuration(TRANSITION_DURATION);
        animation->setEndValue(firstDraggedItemX + i * (SPACING_UNIT + ITEM_WIDTH));
        animation->setEasingCurve(QEasingCurve::InOutQuad);
        draggedItemsAnimation.addAnimation(animation);
    }

    draggedItemsAnimation.start();
    return true;
}

/*!
 * \brief Performs dragging.
 *
 * The \a dPos param holds the mouse position change in scene coordinates.
 */
void AnimationView::drag(QPointF dPos)
{
    qreal firstDraggedItemX = dragItem->childItems().first()->scenePos().x();
    int newDropIndex = (firstDraggedItemX + 0.5 * ITEM_WIDTH) / (SPACING_UNIT + ITEM_WIDTH);
    if (newDropIndex != dropIndex) {
        dropIndex = newDropIndex;
        layOutItems(true);
    }
    dragItem->moveBy(dPos.x(), dPos.y());
}

/*!
 * \brief Finalizes dragging.
 *
 * Applies the new frame orter to the model.
 */
void AnimationView::endDrag()
{
    draggedItemsAnimation.clear();

    // Convert item positions to scene coordinates
    foreach (QGraphicsItem *item, dragItem->childItems())
        item->moveBy(dragItem->x(), dragItem->y());

    // Dismantle the drag item
    dragItem->clearChildItems();
    scene->removeItem(dragItem);
    delete dragItem;
    dragItem = nullptr;
    dropIndex = -1;

    // Aply the reordering to the model
    disconnect(project(), SIGNAL(framesChanged()), this, SLOT(onFramesChanged()));
    sort(items.begin(), items.end(), compareItemsByX);
    for (int i = 0; i < items.count(); ++i)
        project()->moveFrame(items[i]->frame(), i);
    connect(project(), SIGNAL(framesChanged()), this, SLOT(onFramesChanged()));

    layOutItems(true);
}

/*!
 * \brief Handles the framesChanged signal of the displayed project.
 */
void AnimationView::onFramesChanged()
{
    setupScene();
    updateSceneRect();
}

/*!
 * \class AnimationViewItem
 * \inmodule view
 *
 * \brief QGraphicsItem to visualize a frame thumbnail in AnimationView.
 *
 * This class inherits QGraphicsObject, so it can have properties and can be animated using
 * Qt Animation Framework.
 */

// Properties

/*!
 * \property AnimationViewItem::frame
 * \brief The displayed frame.
 */

/*!
 * \property AnimationViewItem::size
 * \brief The dimensions of the item in item coordinates.
 */

/*!
 * \property AnimationViewItem::width
 * \brief The width of the item in item coordinates.
 */

/*!
 * \property AnimationViewItem::height
 * \brief The height of the item in item coordinates.
 */

// Methods

/*!
 * \brief Construct AnimationViewItem displaying \a frame with parent item \a parent.
 */
AnimationViewItem::AnimationViewItem(const AnimationFrame *frame, QGraphicsItem *parent) :
    QGraphicsObject(parent),
    m_size(100, 100),
    m_frame(frame)
{
    // Do nothing
}

/*!
 * \brief Returns the bounds of the item.
 */
QRectF AnimationViewItem::boundingRect() const
{
    return QRectF(0, 0, width(), height());
}

/*!
 * \brief Paints the content of the item using \a painter.
 *
 * Style options are provided in \a option and \a widget, if provided, is the widget being painted
 * on.
 */
void AnimationViewItem::paint(
    QPainter *painter,
    const QStyleOptionGraphicsItem *option,
    QWidget *widget
)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::gray);
    if (isSelected())
        painter->setBrush(Qt::darkGray);
    painter->drawRoundedRect(boundingRect(), SPACING_UNIT, SPACING_UNIT);

    qreal thumbnailScaleX = max(width() - 2 * SPACING_UNIT, 0) / (qreal)m_frame->width();
    qreal thumbnailScaleY = max(height() - 2 * SPACING_UNIT, 0) / (qreal)m_frame->height();
    qreal thumbnailScale = min(thumbnailScaleX, thumbnailScaleY);
    int thumbnailWidth = m_frame->width() * thumbnailScale;
    int thumbnailHeight = m_frame->height() * thumbnailScale;

    QPainterExtensions(painter).drawBackground(
        0.5 * (width() - thumbnailWidth),
        0.5 * (height() - thumbnailHeight),
        thumbnailWidth,
        thumbnailHeight
                );
    QPainterExtensions(painter).drawAnimationFrame(
        0.5 * (width() - thumbnailWidth), 0.5 * (height() - thumbnailHeight),
        thumbnailWidth, thumbnailHeight,
        m_frame
    );
}

