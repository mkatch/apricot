#include "dragtool.hpp"

DragTool::DragTool(QObject *parent) :
    Tool(parent)
{
    // Do nothing
}

void DragTool::mouseMoveEvent(ToolMouseMoveEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        actionArea()->translate(event->dViewPos());
        event->accept();
    }
}

void DragTool::wheelEvent(ToolWheelEvent *event)
{
    qreal factor = 1.0 + event->angleDelta().y() / 1200.0;
    actionArea()->scale(factor);
    event->accept();
}
