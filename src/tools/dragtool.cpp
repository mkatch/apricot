#include "dragtool.hpp"

#include <QDebug>

#include <cstdlib>

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
    } else {
        lastPoint = event->pos().toPoint();
        if (event->buttons() == Qt::RightButton)
            commit();
        else
            preview();
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
    if (event->button() == Qt::RightButton) {
        lastPoint = event->pos().toPoint();
        commit();
        event->accept();
    }
}

void DragTool::paint(Painter *painter, bool preview)
{
    if (preview)
        painter->setPen(QPen(Qt::black, 3));
    else
        painter->setPen(QPen(QColor(rand() % 256, rand() % 256, rand() % 256), 3));
    painter->drawRect(lastPoint.x(), lastPoint.y(), 30, 30);
}
