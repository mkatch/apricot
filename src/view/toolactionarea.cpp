#include "toolactionarea.hpp"

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

void ToolActionArea::mousePressEvent(QMouseEvent *event)
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
    if (event->isAccepted())
        lastMousePos = event->localPos();
}

void ToolActionArea::mouseReleaseEvent(QMouseEvent *event)
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

void ToolActionArea::mouseDoubleClickEvent(QMouseEvent *event)
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

void ToolActionArea::mouseMoveEvent(QMouseEvent *event)
{
    if (tool() == nullptr)
        return event->ignore();

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
    tool()->mouseMoveEvent(&toolEvent);
    lastMousePos = event->localPos();
}

void ToolActionArea::wheelEvent(QWheelEvent *event)
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

void ToolActionArea::keyPressEvent(QKeyEvent *event)
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

void ToolActionArea::keyReleaseEvent(QKeyEvent *event)
{
    if (tool() == nullptr)
        return event->ignore();

    ToolKeyEvent toolEvent(
        event,
        event->key(),
        event->modifiers()
    );
    tool()->keyReleaseEvent(toolEvent);
}
