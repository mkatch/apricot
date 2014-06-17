#include "animationview.hpp"

#include <algorithm>
#include <QGraphicsSceneEvent>
#include <QWheelEvent>
#include <QPropertyAnimation>

#include <ApricotUtils>
#include <ApricotCore>

#include "qpainterextensions.hpp"

using std::sort;

class AnimationViewItem : public QGraphicsObject
{
    Q_OBJECT
    Q_PROPERTY(const AnimationFrame *frame READ frame)
    Q_PROPERTY(const QSize &size READ size WRITE setSize)
    Q_PROPERTY(int width READ width)
    Q_PROPERTY(int height READ height)
    Q_PROPERTY(bool active READ isActive WRITE setActive)

public:

    AnimationViewItem(const AnimationFrame *frame, QGraphicsItem *parent = nullptr);

    const AnimationFrame *frame() const;

    const QSize &size() const;
    void setSize(const QSize &size);
    void setSize(qreal width, qreal height);

    int width() const;
    int height() const;

    bool isActive() const;
    void setActive(bool active);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    static const int SPACING_UNIT = 6;
    static const int ADD_BUTTON_RADIUS = 24;
    static const int PLUS_RADIUS = 8;
    static const int PLUS_WIDTH = 5;
    static const QBrush ACTIVE_BRUSH;
    static const QBrush ADD_DOWN_BRUSH;

    QSize m_size;
    const AnimationFrame *m_frame;
    bool m_active;

    bool addBeforeDown;
    bool addAfterDown;

    AnimationView *view() const;

    friend class AnimationView;
};

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
    graphicsView(new QGraphicsView(this)),
    scene(new QGraphicsScene(this)),
    dragItem(nullptr),
    dropIndex(-1),
    m_project(nullptr),
    m_activeFrame(nullptr)
{
    graphicsView->setRenderHint(QPainter::Antialiasing);
    graphicsView->setScene(scene);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scene->installEventFilter(this);
    scene->setSceneRect(-width() / 2 + 1, -SPACING_UNIT + 1, width() - 2, height() - 2);
    layOut();
}

void AnimationView::setProject(Project *project)
{
    if (m_project == project)
        return;

    m_project = project;
    if (m_project == nullptr) {
        setActiveFrame(nullptr);
        return;
    }

    setupScene();

    QRectF sceneRect = scene->sceneRect();
    if (project->frameCount() > 0) {
        int sceneWidth = items.count() * ITEM_WIDTH
                       + (items.count() + 1) * SPACING_UNIT
                       + 2 * AnimationViewItem::ADD_BUTTON_RADIUS;
        sceneRect.setX(max(0.0, (sceneRect.width() - sceneWidth) / 2)
            - AnimationViewItem::ADD_BUTTON_RADIUS);
        setActiveFrame(project->frame(0));
    } else {
        setActiveFrame(nullptr);
        sceneRect.setX(-width() / 2 + 1);
    }
    scene->setSceneRect(sceneRect);

    connect(project, SIGNAL(framesChanged()), this, SLOT(onFramesChanged()));
}

void AnimationView::setActiveFrame(const AnimationFrame *frame)
{
    if (m_activeFrame == frame)
        return;

    if (m_activeFrame != nullptr) {
        int i = project()->indexOfFrame(m_activeFrame);
        items[i]->setActive(false);
        items[i]->setZValue(0);
    }

    if (frame == nullptr) {
        m_activeFrame = nullptr;
        emit activeFrameChanged(nullptr);
        return;
    }

    if (frame->project() != project()) {
        qWarning(
            "AnimationView::setActiveFrame(): A frame that is not a member of displayed "
            "project was tried to be set as active frame."
        );
        return;
    }

    int i = project()->indexOfFrame(frame);
    m_activeFrame = project()->frame(i);
    items[i]->setActive(true);
    items[i]->setSelected(true);
    items[i]->setZValue(1);

    qDebug() << "emit activeFrameChanged" << m_activeFrame;
    emit activeFrameChanged(m_activeFrame);
}

/*!
 * \brief Occurs when the widget is resized.
 *
 * Event information are provided in \a event.
 */
void AnimationView::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    layOut();
    QRectF sceneRect = scene->sceneRect();
    sceneRect.setWidth(width() - 2);
    sceneRect.setHeight(height() - 2);
    scene->setSceneRect(sceneRect);
}

void AnimationView::wheelEvent(QWheelEvent *event)
{
    int dx = (event->angleDelta().x() + event->angleDelta().y()) / 8;
    scene->setSceneRect(scene->sceneRect().translated(dx, 0));
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
            if (mouse->button() == Qt::LeftButton && dragItem != nullptr)
                endDrag();
            break;
        }
        default: break;
        }
    } else { // object must be one of items
        AnimationViewItem *item = static_cast<AnimationViewItem *>(object);
        if (event->type() == QEvent::GraphicsSceneMousePress) {
            QGraphicsSceneMouseEvent *mouse = static_cast<QGraphicsSceneMouseEvent *>(event);
            if (mouse->button() == Qt::LeftButton) {
                if (scene->selectedItems().empty() || !(mouse->modifiers() & Qt::ControlModifier))
                    setActiveFrame(item->frame());
                return (mouse->modifiers() & Qt::ControlModifier) && item->isActive();
            }
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
    foreach (AnimationFrame *frame, project()->frames())
        items.append(newItem(frame));

    layOutScene(false);
}

AnimationViewItem *AnimationView::newItem(AnimationFrame *frame)
{
    AnimationViewItem *item = new AnimationViewItem(frame);
    item->setFlag(QGraphicsItem::ItemIsSelectable);
    item->installEventFilter(this);
    scene->addItem(item);
    return item;
}

/*! \brief Lays out the child widgets and graphics scene.
 *
 * Called at construction time and after resize.
 */
void AnimationView::layOut()
{
    graphicsView->setGeometry(0, 0, width(), height());
    layOutScene(false);
}

/*!
 * \brief Lays out the scene items.
 *
 * Called in response to rezising and dragging. If \a animate is \c true, item transitions are
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
            qreal itemX = laidOutItemCount * (ITEM_WIDTH + SPACING_UNIT);
            if (dropIndex <= laidOutItemCount)
                itemX += draggedItemCount * (SPACING_UNIT + ITEM_WIDTH);
            if (animate) {
                QPropertyAnimation *animation = new QPropertyAnimation(item, "pos");
                animation->setDuration(TRANSITION_DURATION);
                animation->setEndValue(QPointF(itemX, 0));
                animation->setEasingCurve(QEasingCurve::InOutQuad);
                itemsAnimation.addAnimation(animation);
            } else {
                item->setPos(itemX, 0);
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

    // Apply the reordering to the model
    disconnect(project(), SIGNAL(framesChanged()), this, SLOT(onFramesChanged()));
    sort(items.begin(), items.end(), compareItemsByX);
    for (int i = 0; i < items.count(); ++i)
        project()->moveFrame(items[i]->frame(), i);
    connect(project(), SIGNAL(framesChanged()), this, SLOT(onFramesChanged()));

    layOutItems(true);
}

void AnimationView::handleAddButtonClick(AddButton button)
{
    Q_ASSERT(activeFrame() != nullptr);
    disconnect(project(), SIGNAL(framesChanged()), this, SLOT(onFramesChanged()));

    int i = project()->indexOfFrame(activeFrame());
    QPointF pos = items[i]->pos();
    AnimationViewItem *item;
    if (button == AddBeforeButton) {
        item = newItem(project()->newFrameBefore(i));
        items.insert(i, item);
    }
    else { // if (button == AddAfterButton)
        item = newItem(project()->newFrameAfter(i));
        items.insert(i + 1, item);
    }

    item->setPos(pos); // For animation.
    layOutScene(true);

    connect(project(), SIGNAL(framesChanged()), this, SLOT(onFramesChanged()));
}

/*!
 * \brief Handles the framesChanged signal of the displayed project.
 */
void AnimationView::onFramesChanged()
{
    setupScene();
    //updateSceneRect();
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

// Constants

const QBrush AnimationViewItem::ACTIVE_BRUSH = QBrush(QColor(226, 134, 70));
const QBrush AnimationViewItem::ADD_DOWN_BRUSH = QBrush(QColor(176, 92, 48));

// Methods

/*!
 * \brief Construct AnimationViewItem displaying \a frame with parent item \a parent.
 */
AnimationViewItem::AnimationViewItem(const AnimationFrame *frame, QGraphicsItem *parent) :
    QGraphicsObject(parent),
    m_size(100, 100),
    m_frame(frame),
    m_active(false),
    addBeforeDown(false),
    addAfterDown(false)
{
    // Do nothing
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

inline bool AnimationViewItem::isActive() const
{
    return m_active;
}

inline void AnimationViewItem::setActive(bool active)
{
    m_active = active;
    addBeforeDown = false;
    addAfterDown = false;
    prepareGeometryChange();
}

/*!
 * \brief Returns the bounds of the item.
 */
QRectF AnimationViewItem::boundingRect() const
{
    if (isActive())
        return QRectF(
            -ADD_BUTTON_RADIUS,
            min(height() / 2 - ADD_BUTTON_RADIUS, 0),
            width() + 2 * ADD_BUTTON_RADIUS,
            max(height(), 2 * ADD_BUTTON_RADIUS)
        );
    else
        return QRectF(0, 0, width(), height());
}

QPainterPath AnimationViewItem::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, width(), height());
    if (isActive()) {
        path.addEllipse(QPointF(0, height() / 2), ADD_BUTTON_RADIUS, ADD_BUTTON_RADIUS);
        path.addEllipse(QPointF(width(), height() / 2), ADD_BUTTON_RADIUS, ADD_BUTTON_RADIUS);
    }
    path.setFillRule(Qt::WindingFill);
    return path;
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

    painter->setBrush(Qt::gray);
    if (isSelected())
        painter->setBrush(Qt::darkGray);
    if (isActive())
        painter->setBrush(ACTIVE_BRUSH);
    painter->setPen(Qt::NoPen);
    painter->drawRoundedRect(0, 0, width(), height(), SPACING_UNIT, SPACING_UNIT);

    if (isActive()) {
        // Draw ears
        painter->setBrush(addBeforeDown ? ADD_DOWN_BRUSH : ACTIVE_BRUSH);
        painter->drawEllipse(QPointF(0, height() / 2), ADD_BUTTON_RADIUS, ADD_BUTTON_RADIUS);
        painter->setBrush(addAfterDown ? ADD_DOWN_BRUSH : ACTIVE_BRUSH);
        painter->drawEllipse(QPointF(width(), height() / 2), ADD_BUTTON_RADIUS, ADD_BUTTON_RADIUS);
        // Draw pluses
        painter->setPen(QPen(Qt::white, PLUS_WIDTH, Qt::SolidLine, Qt::FlatCap));
        QPointF lpc(-PLUS_RADIUS, height() / 2);
        QPointF rpc(width() + PLUS_RADIUS, height() / 2);
        painter->drawLine(lpc.x() - PLUS_RADIUS, lpc.y(), lpc.x() + PLUS_RADIUS, lpc.y());
        painter->drawLine(lpc.x(), lpc.y() - PLUS_RADIUS, lpc.x(), lpc.y() + PLUS_RADIUS);
        painter->drawLine(rpc.x() - PLUS_RADIUS, rpc.y(), rpc.x() + PLUS_RADIUS, rpc.y());
        painter->drawLine(rpc.x(), rpc.y() - PLUS_RADIUS, rpc.x(), rpc.y() + PLUS_RADIUS);
    }

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

void AnimationViewItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsObject::mousePressEvent(event);
    if (!isActive() || event->button() != Qt::LeftButton)
        return;
    if (event->pos().x() <= 0)
        addBeforeDown = true;
    if (event->pos().x() >= width())
        addAfterDown = true;
    update();
}

void AnimationViewItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsObject::mouseReleaseEvent(event);
    if (!isActive() || event->button() != Qt::LeftButton)
        return;
    if (addBeforeDown && event->pos().x() <= 0)
        view()->handleAddButtonClick(AnimationView::AddBeforeButton);
    if (addAfterDown && event->pos().x() >= width())
        view()->handleAddButtonClick(AnimationView::AddAfterButton);
    addBeforeDown = false;
    addAfterDown = false;
    update();
}

inline AnimationView *AnimationViewItem::view() const
{
    return static_cast<AnimationView *>(scene()->parent());
}

#include "animationview.moc"
#include "moc_animationview.cpp"
