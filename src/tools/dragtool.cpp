#include "dragtool.hpp"

DragTool::DragTool(QObject *parent) :
    Tool(parent)
{
    // Do nothing
}

void DragTool::mouseMoveEvent(ToolMouseMoveEvent *event)
{
    if (event->buttons() == Qt::LeftButton) {
        view()->translate(event->dViewPos());
        event->accept();
    }
}

void DragTool::wheelEvent(ToolWheelEvent *event)
{
    qreal df = event->angleDelta().y() / 1200.0;
    QPointF center(0.5 * view()->width(), 0.5 * view()->height());
    view()->scale(1.0 + df);
    view()->translate(df * (view()->translation() - center));
    event->accept();
}
