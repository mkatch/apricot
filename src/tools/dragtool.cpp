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
    qreal factor = 1.0 + event->angleDelta().y() / 1200.0;
    qreal oldScale = view()->scale();
    view()->scale(factor);
    qreal f = factor / oldScale - 1.0;
    QPointF center(view()->width() / 4.0, view()->height() / 4.0);
    view()->translate(f * (view()->pos() - center));
    event->accept();
}
