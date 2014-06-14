#include "layerview.hpp"


#include <algorithm>
#include <QGraphicsSceneEvent>
#include <QPropertyAnimation>

#include <ApricotUtils>
#include <ApricotCore>

#include "qpainterextensions.hpp"

using std::sort;

/*!
 * \class LayerView
 * \inmodule view
 *
 * \brief A widget to manage layers
 */

 // Properties

/*!
 * \property LayerView::frame
 * \brief The frame the layers of which are displayed.
 */

// Methods

/*!
 * \brief Constructs LayerView with parent widget \a parent.
 */
LayerView::LayerView(QWidget *parent) :
    QWidget(parent),
    graphicsView(new QGraphicsView(this)),
    scene(new QGraphicsScene(this)),
    dragItem(nullptr),
    dropIndex(-1),
    add(new QPushButton("Add", this)),
    remove(new QPushButton("Remove", this)),
    m_frame(nullptr),
    m_activeLayer(nullptr)

{
    setMinimumWidth(150);
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setRenderHint(QPainter::Antialiasing);
    graphicsView->setScene(scene);
    scene->installEventFilter(this);

    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->addWidget(graphicsView);
    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(add);
    hbox->addWidget(remove);
    vbox->addLayout(hbox);
    setLayout(vbox);
    layOut();
    connect(add, SIGNAL(clicked()), this, SLOT(addLayer()));
    connect(remove, SIGNAL(clicked()), this, SLOT(removeLayer()));

}

void LayerView::setFrame(AnimationFrame *frame)
{
    if (m_frame == frame)
        return;

    m_frame = frame;
    connect(frame, SIGNAL(layersChanged()), this, SLOT(onLayersChanged()));
    setupScene();
    setActiveLayer(frame->layer(0));
//    emit layersChanged();
}

void LayerView::setActiveLayer(const Layer *layer)
{
    if (m_activeLayer == layer)
        return;

    if(layer->frame() != frame()) {
        qWarning(
            "LayerView::setActiveLayer(): A layer that is not a member of displayed "
            "frame was tried to be set as active frame."
        );
        return;
    }

    m_activeLayer = const_cast<Layer *>(layer);
    emit activeLayerChanged(m_activeLayer);
}

/*!
 * \brief Occurs when the widget is resized.
 *
 * Event information are provided in \a event.
 */
void LayerView::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    layOut();
    updateSceneRect();
}

/*!
 * \brief Filters events if this view has been installed as an event filter for \a object.
 *
 * This is an overiden method. LayerView is installed as an event filter for it's underlying
 * QGraphicsScene. This is done to handle dragging and does not reject any \a event. This
 * implementation always returns \c false which means all events are passed further.
 */
bool LayerView::eventFilter(QObject *object, QEvent *event)
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
    } else {
       LayerViewItem *item = static_cast<LayerViewItem *>(object);
       if (event->type() == QEvent::GraphicsSceneMousePress) {
           QGraphicsSceneMouseEvent *mouse = static_cast<QGraphicsSceneMouseEvent *>(event);
           if (mouse->button() == Qt::LeftButton)
               setActiveLayer(item->layer());
       }
    }

    return false;
}


/*!
 * \brief Creates the scene.
 */
void LayerView::setupScene()
{
    // Remove current thumbnails
    for (int i = 0; i < items.count(); ++i) {
        scene->removeItem(items[i]);
        delete items[i];
    }
    items.clear();

    if (frame() == nullptr)
        return;

    // Add new thumbnails
    for (int i = 0; i < frame()->layerCount(); ++i) {
        LayerViewItem *item = new LayerViewItem(frame()->layer(i));
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
void LayerView::updateSceneRect()
{
    graphicsView->setSceneRect(
        0, 0,
        width(),
        (items.count() + 1) * SPACING_UNIT + items.count() * ITEM_HEIGHT
    );
}

/*! \brief Lays out the child widgets and graphics scene.
 *
 * Called at construction time and after resize.
 */
void LayerView::layOut()
{
//    graphicsView->setGeometry(0, 0, width(), height());
    layOutScene(false);
}

/*!
 * \brief Lays out the scene items.
 *
 * Called in response to resizing. If \a animate is \c true, item transitions are
 * animated. Otherwise the change is immediate.
 */
void LayerView::layOutScene(bool animate)
{
    if (frame() == nullptr)
        return;

    foreach (LayerViewItem *item, items)
        item->setSize(max(width() - 2 * SPACING_UNIT, 0), ITEM_HEIGHT);

    layOutItems(animate);
}

/*!
 * \brief Lays out scene items that represent the layers.
 *
 * Called in response to rezising and dragging. If \a animete is \c true, item transitions are
 * animated. Otherwise the change is immediate.
 */
void LayerView::layOutItems(bool animate)
{
    itemsAnimation.clear();

    int draggedItemCount = (dragItem != nullptr) ? dragItem->childItems().count() : 0;
    int laidOutItemCount = 0;
    foreach (LayerViewItem *item, items)
        if (dragItem == nullptr || item->parentItem() != dragItem) {
            qreal itemY = SPACING_UNIT + laidOutItemCount * (ITEM_HEIGHT + SPACING_UNIT);
            if (dropIndex <= laidOutItemCount)
                itemY += draggedItemCount * (SPACING_UNIT + ITEM_HEIGHT);
            if (animate) {
                QPropertyAnimation *animation = new QPropertyAnimation(item, "pos");
                animation->setDuration(TRANSITION_DURATION);
                animation->setEndValue(QPointF(SPACING_UNIT, itemY));
                animation->setEasingCurve(QEasingCurve::InOutQuad);
                itemsAnimation.addAnimation(animation);
            } else
                item->setY(itemY);
            ++laidOutItemCount;
        }

    if (itemsAnimation.animationCount() > 0)
        itemsAnimation.start();
}

bool compareItemsByY(QGraphicsItem *i, QGraphicsItem *j)
{
    return i->y() < j->y();
}

/*!
 * \brief Initiates dragging.
 *
 * The mouse position at the moment of pressing is passed in \a dragBeginPos. The method returns
 * \c false if the conditions for dragging are not fulfilled.
 */
bool LayerView::tryBeginDrag(QPointF dragBeginPos)
{
    QGraphicsItem *grabbedItem = scene->itemAt(dragBeginPos, graphicsView->transform());
    if (grabbedItem == NULL || !grabbedItem->isSelected())
        return false;
    // Only layer items are selectable so if we reached this, it means the user grabbed a layer

    QList<QGraphicsItem *> draggedItems = scene->selectedItems();
    sort(draggedItems.begin(), draggedItems.end(), compareItemsByY);
    dragItem = new GraphicsEmptyItem;
    scene->addItem(dragItem);
    dragItem->addChildItems(draggedItems);
    dragItem->setZValue(1);

    int grabbedItemIndex = draggedItems.indexOf(grabbedItem);
    qreal firstDraggedItemY = grabbedItem->y() - grabbedItemIndex * (SPACING_UNIT + ITEM_HEIGHT);
    for (int i = 0; i < draggedItems.count(); ++i) {
        LayerViewItem *item = static_cast<LayerViewItem *>(draggedItems[i]);
        QPropertyAnimation *animation = new QPropertyAnimation(item, "y");
        animation->setDuration(TRANSITION_DURATION);
        animation->setEndValue(firstDraggedItemY + i * (SPACING_UNIT + ITEM_HEIGHT));
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
void LayerView::drag(QPointF dPos)
{
    qreal firstDraggedItemY = dragItem->childItems().first()->scenePos().y();
    int newDropIndex = (firstDraggedItemY + 0.5 * ITEM_HEIGHT) / (SPACING_UNIT + ITEM_HEIGHT);
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
void LayerView::endDrag()
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
    disconnect(frame(), SIGNAL(layersChanged()), this, SLOT(onLayersChanged()));
    sort(items.begin(), items.end(), compareItemsByY);
    for (int i = 0; i < items.count(); ++i)
        frame()->moveLayer(items[i]->layer(), i);
    connect(frame(), SIGNAL(layersChanged()), this, SLOT(onLayersChanged()));

    layOutItems(true);
}

/*!
 * \brief Handles the layersChanged signal of the displayed frame.
 */
void LayerView::onLayersChanged()
{
    setupScene();
    updateSceneRect();
}

void LayerView::addLayer()
{
    if(activeLayer() == nullptr)
        frame()->newLayer(0);
    else {
        int indexOfLayer = frame()->indexOfLayer(activeLayer());
        frame()->newLayer(indexOfLayer+1);
    }
    setupScene();
    updateSceneRect();
}

void LayerView::removeLayer()
{
    Layer *layer;
    if(activeLayer() != nullptr){
        int indexOfLayer = frame()->indexOfLayer(activeLayer());
        frame()->deleteLayer(indexOfLayer);

        if(frame()->layerCount() == 0){
            layer = frame()->newLayer();
            setActiveLayer(layer);
        } else {
            layer = frame()->layer(0);
            setActiveLayer(layer);
        }

        emit activeLayerChanged(layer);
        setupScene();
        updateSceneRect();
    }
}

/*!
 * \class LayerViewItem
 * \inmodule view
 *
 * \brief QGraphicsItem to visualize a frame thumbnail in LayerView.
 *
 * This class inherits QGraphicsObject, so it can have properties and can be animated using
 * Qt Animation Framework.
 */

// Properties

/*!
 * \property LayerViewItem::layer
 * \brief The displayed layer.
 */

/*!
 * \property LayerViewItem::size
 * \brief The dimensions of the item in item coordinates.
 */

/*!
 * \property LayerViewItem::width
 * \brief The width of the item in item coordinates.
 */

/*!
 * \property LayerViewItem::height
 * \brief The height of the item in item coordinates.
 */

// Methods

/*!
 * \brief Construct LayerViewItem displaying \a layer with parent item \a parent.
 */
LayerViewItem::LayerViewItem(const Layer *layer, QGraphicsItem *parent) :
    QGraphicsObject(parent),
    m_size(100, 100),
    m_layer(layer)
{
    // Do nothing
}

/*!
 * \brief Returns the bounds of the item.
 */
QRectF LayerViewItem::boundingRect() const
{
    return QRectF(0, 0, width(), height());
}

/*!
 * \brief Paints the content of the item using \a painter.
 *
 * Style options are provided in \a option and \a widget, if provided, is the widget being painted
 * on.
 */
void LayerViewItem::paint(
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

    qreal thumbnailScaleX = max(width() - 2 * SPACING_UNIT, 0) / (qreal)m_layer->width();
    qreal thumbnailScaleY = max(height() - 2 * SPACING_UNIT, 0) / (qreal)m_layer->height();
    qreal thumbnailScale = min(thumbnailScaleX, thumbnailScaleY);
    int thumbnailWidth = m_layer->width() * thumbnailScale;
    int thumbnailHeight = m_layer->height() * thumbnailScale;

    QPainterExtensions(painter).drawBackground(
        0.5 * (width() - thumbnailWidth) - 5,
        0.5 * (height() - thumbnailHeight),
        thumbnailWidth,
        thumbnailHeight
    );
    QPainterExtensions(painter).drawLayer(
        0.5 * (width() - thumbnailWidth) - 5,
        0.5 * (height() - thumbnailHeight),
        thumbnailWidth,
        thumbnailHeight,
        m_layer
    );
}
