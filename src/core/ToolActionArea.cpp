#include "ToolActionArea.hpp"
#include "Tool.hpp"
#include "ToolEvents.hpp"

#include <QDebug>


ToolActionArea::ToolActionArea(QWidget *parent) :
    CanvasView(parent),
    m_tool(nullptr)
{
    this->setMouseTracking(true);
}


void ToolActionArea::setTool(Tool *tool)
{
    if (m_tool == tool)
        return;

    bool emitActiveChanged = (tool == nullptr) || (m_tool == nullptr);
    if (m_tool != nullptr)
        m_tool->setActionArea(nullptr);
    m_tool = tool;
    m_tool->setActionArea(this);
    emit toolChanged();
    update();
    if (emitActiveChanged)
        emit activeChanged();
}


QPointF ToolActionArea::canvasToArea(const QPoint &canvasPos) const
{
    return canvasPos;
}


QPoint ToolActionArea::areaToCanvas(const QPointF &areaPos) const
{
    return areaPos.toPoint();
}


bool ToolActionArea::event(QEvent *event)
{
    if (!active()) {
        event->ignore();
        return false;
    }

    switch (event->type()) {
    case QEvent::MouseButtonPress:
        dispatchMousePressEvent(static_cast<QMouseEvent *>(event));
        break;

    case QEvent::MouseButtonRelease:
        dispatchMouseReleaseEvent(static_cast<QMouseEvent *>(event));
        break;

    case QEvent::MouseButtonDblClick:
        dispatchMouseDoubleClickEvent(static_cast<QMouseEvent *>(event));
        break;

    case QEvent::MouseMove:
        dispatchMouseMoveEvent(static_cast<QMouseEvent *>(event));
        break;

    case QEvent::HoverEnter:
    case QEvent::HoverLeave:
    case QEvent::HoverMove:
        dispatchHoverMoveEvent(static_cast<QHoverEvent *>(event));
        break;

    case QEvent::Wheel:
        dispatchWheelEvent(static_cast<QWheelEvent *>(event));
        break;

    default:
        return CanvasView::event(event);
    }

    return true;
}

void ToolActionArea::dispatchMousePressEvent(QMouseEvent *event)
{
    ToolMouseEvent toolEvent(
        event,
        areaToCanvas(event->localPos()),
        event->localPos(),
        event->button(),
        event->buttons(),
        event->modifiers()
    );
    tool()->mousePressEvent(&toolEvent);
    if (event->isAccepted())
        lastMousePos = event->localPos();
}


void ToolActionArea::dispatchMouseReleaseEvent(QMouseEvent *event)
{
    ToolMouseEvent toolEvent(
        event,
        areaToCanvas(event->localPos()),
        event->localPos(),
        event->button(),
        event->buttons(),
        event->modifiers()
    );
    tool()->mouseReleaseEvent(&toolEvent);
}


void ToolActionArea::dispatchMouseDoubleClickEvent(QMouseEvent *event)
{
    ToolMouseEvent toolEvent(
        event,
        areaToCanvas(event->localPos()),
        event->localPos(),
        event->button(),
        event->buttons(),
        event->modifiers()
    );
    tool()->mouseDoubleClickEvent(&toolEvent);
}


void ToolActionArea::dispatchMouseMoveEvent(QMouseEvent *event)
{
    ToolMouseMoveEvent toolEvent(
        event,
        areaToCanvas(event->localPos()),
        areaToCanvas(lastMousePos),
        event->localPos(),
        lastMousePos,
        event->button(),
        event->buttons(),
        event->modifiers()
    );
    tool()->mouseDragEvent(&toolEvent);
    lastMousePos = event->localPos();
}


void ToolActionArea::dispatchHoverMoveEvent(QHoverEvent *event)
{
    ToolMouseMoveEvent toolEvent(
        event,
        areaToCanvas(event->posF()),
        areaToCanvas(event->oldPosF()),
        event->posF(),
        event->oldPosF(),
        Qt::NoButton,
        Qt::NoButton,
        event->modifiers()
    );
    tool()->mouseHoverEvent(&toolEvent);
}


void ToolActionArea::dispatchWheelEvent(QWheelEvent *event)
{
    ToolMouseWheelEvent toolEvent(
        event,
        areaToCanvas(event->posF()),
        event->posF(),
        event->pixelDelta(),
        event->angleDelta(),
        event->buttons(),
        event->modifiers()
    );
    tool()->mouseWheelEvent(&toolEvent);
}
