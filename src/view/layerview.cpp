#include "layerview.hpp"


#include <algorithm>
#include <QGraphicsSceneEvent>
#include <QPropertyAnimation>

#include <ApricotUtils>
#include <ApricotCore>

#include "qpainterextensions.hpp"

using std::sort;

LayerView::LayerView(QWidget *parent) :
    QWidget(parent),
    graphicsView(new QGraphicsView(this)),
    scene(new QGraphicsScene(this)),
    dragItem(nullptr),
    dropIndex(-1)
{
    setMinimumWidth(170);
    graphicsView->setRenderHint(QPainter::Antialiasing);
    graphicsView->setScene(scene);
    scene->installEventFilter(this);
    layOut();
}

void LayerView::setFrame(AnimationFrame *frame)
{
    if (m_frame == frame)
        return;

    m_frame = frame;
    connect(frame, SIGNAL(layersChanged()), this, SLOT(onLayersChanged()));
    setupScene();
    emit layersChanged();
}

void LayerView::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    layOut();
    updateSceneRect();
}

// TODO look up event filter
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
    }

    return false;
}


// TODO repair this
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
        scene->addItem(item);
        item->setFlag(QGraphicsItem::ItemIsSelectable);
        items.append(item);
    }

    layOutScene(false); // NOTE look up layOutScene
}

void LayerView::updateSceneRect()
{
    graphicsView->setSceneRect(
        0, 0,
        (items.count() + 1) * SPACING_UNIT + items.count() * ITEM_WIDTH,
        height()
    );
}


void LayerView::layOut()
{
    graphicsView->setGeometry(0, 0, width(), height());
    layOutScene(false);
}


void LayerView::layOutScene(bool animate)
{
    if (frame() == nullptr)
        return;

    foreach (LayerViewItem *item, items){
        item->setSize(ITEM_WIDTH, ITEM_HEIGHT);//max(height() - 2 * SPACING_UNIT, 0));

    }

    layOutItems(animate);
}

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
            } else {
                item->setY(itemY);
            }
            ++laidOutItemCount;
        }

    if (itemsAnimation.animationCount() > 0)
        itemsAnimation.start();
}

bool compareItemsByY(QGraphicsItem *i, QGraphicsItem *j)
{
    return i->y() < j->y();
}

bool LayerView::tryBeginDrag(QPointF dragBeginPos)
{
    QGraphicsItem *grabbedItem = scene->itemAt(dragBeginPos, graphicsView->transform());
    if (!grabbedItem->isSelected())
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

    layOutItems(true); // NOTE look up layOutItmes
}

void LayerView::onLayersChanged()
{
    setupScene();
    updateSceneRect();
}










LayerViewItem::LayerViewItem(const Layer *layer, QGraphicsItem *parent) :
    QGraphicsObject(parent),
    m_size(100, 100),
    m_layer(layer)
{
    // Do nothing
}

QRectF LayerViewItem::boundingRect() const
{
    return QRectF(0, 0, width(), height());
}

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

    QPainterExtensions(painter).drawLayer(
        0.5 * (width() - thumbnailWidth),
        0.5 * (height() - thumbnailHeight),
        thumbnailWidth,
        thumbnailHeight,
        m_layer
    );
}
