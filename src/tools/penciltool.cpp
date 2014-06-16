#include "penciltool.hpp"

PencilTool::PencilTool(QObject *parent) :
    Tool(parent), pressed(false)
{
    // Do nothing.
}

void PencilTool::mouseMoveEvent(ToolMouseMoveEvent *event)
{
    mousePosition = event->pos().toPoint();
    preview();

    if (pressed)
        commit();
}

void PencilTool::mousePressEvent(ToolMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        commit();
        pressed = true;
        event->accept();
    }
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
