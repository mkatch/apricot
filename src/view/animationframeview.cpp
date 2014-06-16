#include "animationframeview.hpp"

#include <algorithm>
using std::max;

#include <ApricotUtils>

#include "tool.hpp"
#include "toolevents.hpp"
#include "qpainterextensions.hpp"

class GraphicsAnimationFrameViewItem : public QGraphicsItem
{
public:
    GraphicsAnimationFrameViewItem(const AnimationFrameView *view);

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    static const int PLACEHOLDER_SIZE = 128;

    const AnimationFrameView *view;
};

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
 * \property AnimationFrameView::activeLayer
 * \brief The layer to which tool drawing actions are applied.
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
    frameItem(new GraphicsAnimationFrameViewItem(this)),
    m_frame(nullptr),
    m_activeLayer(nullptr),
    m_tool(nullptr),
    onionSkin(nullptr)
{
    this->setMouseTracking(true);
    scene->addItem(frameItem);

    graphicsView->setScene(scene);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    graphicsView->setStyleSheet("background: darkgray");

    background = scene->addRect(frameItem->sceneBoundingRect());
    background->setBrush(QPainterExtensions(new QPainter()).getBrushForBackground());
    background->setZValue(-1);

    onionSkinPrevious = 1;
    onionSkinNext = 1;

    layOut();
}

/*!
 * \fn AnimationFrameView::~AnimationFrameView()
 * \brief Destroys the view.
 */

void AnimationFrameView::setScale(qreal scale)
{
    if (frameItem->scale() == scale)
        return;

    frameItem->setScale(scale);
    background->setRect(frameItem->sceneBoundingRect());
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
    background->setRect(frameItem->sceneBoundingRect());
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
 * \fn AnimationFrameView::mapToFrame(const QPointF &point) const
 * \brief Maps \a point from view coordinates to frame coordinates.
 *
 * \sa mapFromFrame()
 */

/*!
 * \fn AnimationFrameView::mapFromFrame(const QPointF &point) const
 * \brief Maps \a point from frame coordinates to view coordinates.
 *
 * \sa mapToFrame()
 */

void AnimationFrameView::setFrame(AnimationFrame *frame)
{
    if (m_frame == frame)
        return;

    m_frame = frame;
    if (frame != nullptr)
        setActiveLayer(frame->layer(0));
    background->setRect(frameItem->sceneBoundingRect());
    frameItem->update();
    setOnionSkinFrames();
    emit frameChanged();
    update();
}

void AnimationFrameView::setActiveLayer(Layer *layer)
{
    if (m_activeLayer == layer)
        return;

    if (layer != nullptr && layer->frame() != frame()) {
        qWarning(
            "AnimationFrameView::setActiveLayer(): "
            "Passed layer is not part of currently displayed frame"
        );
        return setActiveLayer(nullptr);
    }

    m_activeLayer = layer;
    backBuffer = (layer != nullptr) ? layer->canvas() : Canvas();
    lastBackBufferChange = QRect();
    emit activeLayerChanged();
    update();
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

void AnimationFrameView::setOnionSkinFrames()
{

    if(onionSkinPrevious > 0 && onionSkinNext) {
        QList<AnimationFrame *> onionSkinFrames;
        int index = frame()->project()->frames().indexOf(frame());
        int beginOfOnionSkin = max(0, index-onionSkinPrevious);
        int endOfOnionSkin = min(frame()->project()->frameCount(), index+onionSkinNext);
        for(int i=beginOfOnionSkin;i<index;i++) {
            onionSkinFrames.push_back(frame()->project()->frame(i));
        }
        for(int i=index+1;i<=endOfOnionSkin;i++) {
            onionSkinFrames.push_back(frame()->project()->frame(i));
        }
        if(onionSkinFrames.size() > 0) {
            onionSkin = new QPixmap(frameItem->scene()->width(), frameItem->scene()->height());
            onionSkin->fill(Qt::transparent);
            QPainter *p = new QPainter(onionSkin);

            p->setOpacity(0.5);
            for (int i = 0; i < onionSkinFrames.size(); ++i) {
                AnimationFrame *f = onionSkinFrames.at(i);
                for (int j = f->layerCount() - 1; j >= 0; --j) {
                    const Layer *layer = f->layer(j);
                    const Canvas& canvas = (layer->canvas().pixmap());
                    p->drawPixmap(0, 0, canvas.pixmap());
                }
            }

            repaint();
        }
    }
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

void AnimationFrameView::revertBackBuffer()
{
    Painter painter(backBuffer);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.drawCanvas(
        lastBackBufferChange.topLeft(),
        activeLayer()->canvas(),
        lastBackBufferChange
    );
    frameItem->update(lastBackBufferChange);
    lastBackBufferChange = QRect();
}

void AnimationFrameView::toolPreview()
{
    revertBackBuffer();
    Painter painter(backBuffer);
    m_tool->paint(&painter, true);
    lastBackBufferChange = painter.boundingBox();
    frameItem->update(lastBackBufferChange);
}

void AnimationFrameView::toolCommit()
{
    revertBackBuffer();
    Painter backPainter(backBuffer);
    m_tool->paint(&backPainter, false);

    Painter *frontPainter = m_activeLayer->beginPainting();
    frontPainter->drawCanvas(
        backPainter.boundingBox().topLeft(),
        backBuffer,
        backPainter.boundingBox()
    );
    m_activeLayer->endPainting();

    lastBackBufferChange = QRect();
    frameItem->update(backPainter.boundingBox());
}

GraphicsAnimationFrameViewItem::GraphicsAnimationFrameViewItem(
    const AnimationFrameView *view
) :
    view(view)
{
    // Do nothing
}

QRectF GraphicsAnimationFrameViewItem::boundingRect() const
{
    return (view->frame() != nullptr)
        ? QRectF(QPoint(0, 0), view->frame()->size())
        : QRectF(0, 0, PLACEHOLDER_SIZE, PLACEHOLDER_SIZE);
}

void GraphicsAnimationFrameViewItem::paint(
    QPainter *painter,
    const QStyleOptionGraphicsItem *option,
    QWidget *widget
)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if (view->onionSkin != nullptr)
        painter->drawPixmap(0,0,*(view->onionSkin));
    if (view->frame() != nullptr) {

        for (int i = view->frame()->layerCount() - 1; i >= 0; --i) {
            const Layer *layer = view->frame()->layer(i);
            const Canvas& canvas = (layer != view->activeLayer())
                ? layer->canvas().pixmap()
                : view->backBuffer;
            painter->drawPixmap(0, 0, canvas.pixmap());
        }
    } else {
        painter->fillRect(0, 0, PLACEHOLDER_SIZE, PLACEHOLDER_SIZE, Qt::black);
    }
}
