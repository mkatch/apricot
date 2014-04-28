#include "dragtool.hpp"

DragTool::DragTool(QObject *parent) :
    Tool(parent)
{
    // Do nothing
}

void DragTool::mousePressEvent(ToolMouseEvent *event)
{
    event->accept(); // This is needed to initiate drag
}

void DragTool::mouseDragEvent(ToolMouseMoveEvent *event)
{
    actionArea()->dragBy(event->dAreaPos());
    event->accept();
}

void DragTool::mouseWheelEvent(ToolMouseWheelEvent *event)
{
    qreal factor = 1.0 + event->angleDelta().y() / 1200.0;
    actionArea()->setZoom(actionArea()->zoom() * factor);
    event->accept();
}
