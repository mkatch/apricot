#include "ToolActionArea.hpp"

#include <QDebug>

#include "Tool.hpp"
#include "ToolEvents.hpp"

ToolActionArea::ToolActionArea(QQuickItem *parent) :
    CanvasView(parent),
    m_tool(nullptr),
    toolChangedSinceLastUpdatePaintNode(false)
{
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::AllButtons);
    setFlag(QQuickItem::ItemHasContents, true);
    setFlag(QQuickItem::ItemIsFocusScope, true);
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
    toolChangedSinceLastUpdatePaintNode = true;
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

void ToolActionArea::mousePressEvent(QMouseEvent *event)
{
    if (!active())
    {
        event->ignore();
        return;
    }

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

void ToolActionArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (!active())
    {
        event->ignore();
        return;
    }

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

void ToolActionArea::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (!active())
    {
        event->ignore();
        return;
    }

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

void ToolActionArea::mouseMoveEvent(QMouseEvent *event)
{
    if (!active())
    {
        event->ignore();
        return;
    }

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

void ToolActionArea::hoverMoveEvent(QHoverEvent *event)
{
    if (!active())
    {
        event->ignore();
        return;
    }

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

void ToolActionArea::wheelEvent(QWheelEvent *event)
{
    if (!active())
    {
        event->ignore();
        return;
    }

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

QSGNode *ToolActionArea::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data)
{
    Q_UNUSED(data)
    if (!active())
        return nullptr;

    if (toolChangedSinceLastUpdatePaintNode)
    {
        delete oldNode;
        oldNode = nullptr;
        toolChangedSinceLastUpdatePaintNode = false;
    }
    return tool()->updateActionAreaPaintNode(oldNode);
}
