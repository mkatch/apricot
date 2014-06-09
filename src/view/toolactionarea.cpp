#include "toolactionarea.hpp"

#include <ApricotUtils>

#include "tool.hpp"
#include "toolevents.hpp"

ToolActionArea::ToolActionArea(QWidget *parent) :
    AnimationFrameView(parent),
    m_tool(nullptr),
    pressed(false)
{
    this->setMouseTracking(true);
}

void ToolActionArea::setTool(Tool *tool)
{
    if (m_tool == tool)
        return;

    if (m_tool != nullptr)
        m_tool->setActionArea(nullptr);
    m_tool = tool;
    m_tool->setActionArea(this);
    emit toolChanged();
    update();
}

bool ToolActionArea::event(QEvent *event)
{
    if (tool() == nullptr)
        return AnimationFrameView::event(event);

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
        return AnimationFrameView::event(event);
    }

    return true;
}

void ToolActionArea::dispatchMousePressEvent(QMouseEvent *event)
{
    ToolMouseEvent toolEvent(
        event,
        mapToFrame(event->localPos()),
        event->localPos(),
        event->button(),
        event->buttons(),
        event->modifiers()
    );
    tool()->mousePressEvent(&toolEvent);
    if (event->isAccepted())
        lastMousePos = event->localPos();
    if(event->button() == Qt::LeftButton)
        pressed = true;
}

void ToolActionArea::dispatchMouseReleaseEvent(QMouseEvent *event)
{
    ToolMouseEvent toolEvent(
        event,
        mapToFrame(event->localPos()),
        event->localPos(),
        event->button(),
        event->buttons(),
        event->modifiers()
    );
    tool()->mouseReleaseEvent(&toolEvent);
    pressed = false;
}

void ToolActionArea::dispatchMouseDoubleClickEvent(QMouseEvent *event)
{
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

void ToolActionArea::dispatchMouseMoveEvent(QMouseEvent *event)
{
    if(pressed)
    {
        ToolMouseMoveEvent toolEvent(
            event,
            mapToFrame(event->localPos()),
            mapToFrame(lastMousePos),
            event->localPos(),
            lastMousePos,
            event->button(),
            event->buttons(),
            event->modifiers()
        );
        tool()->mouseDragEvent(&toolEvent);
        lastMousePos = event->localPos();
    }
}

void ToolActionArea::dispatchHoverMoveEvent(QHoverEvent *event)
{
    ToolMouseMoveEvent toolEvent(
        event,
        mapToFrame(event->posF()),
        mapToFrame(event->oldPosF()),
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
        mapToFrame(event->posF()),
        event->posF(),
        event->pixelDelta(),
        event->angleDelta(),
        event->buttons(),
        event->modifiers()
    );
    tool()->mouseWheelEvent(&toolEvent);
}
