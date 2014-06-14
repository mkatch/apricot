#include "penciltool.hpp"

PencilTool::PencilTool(QObject *parent) :
    Tool(parent)
{
    // Do nothing.
}

void PencilTool::mouseMoveEvent(ToolMouseMoveEvent *event)
{
    mousePosition = event->pos().toPoint();

    if (pressed)
        commit();
    else
        preview();
}

void PencilTool::mousePressEvent(ToolMouseEvent *event)
{
    Q_UNUSED(event)

    if (event->button() == Qt::LeftButton)
        pressed = true;
}

void PencilTool::mouseReleaseEvent(ToolMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
        pressed = false;
}

void PencilTool::paint(Painter *painter, bool preview)
{
    Q_UNUSED(preview)
    painter->drawPoint(mousePosition);
}
