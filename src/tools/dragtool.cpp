#include "dragtool.hpp"

#include <QDebug>

DragTool::DragTool(QObject *parent) :
    Tool(parent)
{
    // Do nothing
}

void DragTool::mouseMoveEvent(ToolMouseMoveEvent *event)
{
    if (event->buttons() & Qt::LeftButton) {
        view()->translate(event->dViewPos());
        event->accept();
    }
}

void DragTool::wheelEvent(ToolWheelEvent *event)
{
    qreal factor = 1.0 + event->angleDelta().y() / 1200.0;
    view()->scale(factor);
    event->accept();
}

void DragTool::mousePressEvent(ToolMouseEvent *event)
{
    lastPoint = event->pos().toPoint();
    preview();
    event->accept();
}

void DragTool::paint(Painter *painter, bool preview)
{
    Q_UNUSED(preview)
    painter->drawRect(lastPoint.x(), lastPoint.y(), 10, 10);
}
