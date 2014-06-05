#include "toolactionarea.hpp"

#include <ApricotUtils>
#include "tool.hpp"
#include "toolevents.hpp"


ToolActionArea::ToolActionArea(QWidget *parent) :
    CanvasView(parent),
    m_tool(nullptr),
    pressed(false)
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


bool ToolActionArea::event(QEvent *event)
{
    if (!active()) {
        return CanvasView::event(event);
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
        qDebug() << "hello lady";
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
        mapToCanvas(event->localPos()),
        event->localPos(),
        event->button(),
        event->buttons(),
        event->modifiers()
    );
    tool()->mousePressEvent(&toolEvent);
    if (event->isAccepted())
        lastMousePos = event->localPos();
    if(event->button() == Qt::LeftButton) {
        pressed = true;
    }

}


void ToolActionArea::dispatchMouseReleaseEvent(QMouseEvent *event)
{
    ToolMouseEvent toolEvent(
        event,
        mapToCanvas(event->localPos()),
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
        mapToCanvas(event->localPos()),
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
            mapToCanvas(event->localPos()),
            mapToCanvas(lastMousePos),
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
        mapToCanvas(event->posF()),
        mapToCanvas(event->oldPosF()),
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
        mapToCanvas(event->posF()),
        event->posF(),
        event->pixelDelta(),
        event->angleDelta(),
        event->buttons(),
        event->modifiers()
    );
    tool()->mouseWheelEvent(&toolEvent);
}
