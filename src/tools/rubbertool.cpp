#include "rubbertool.hpp"

RubberTool::RubberTool(QObject *parent) :
    Tool(parent)
{
    // Do nothing.
}

void RubberTool::mouseMoveEvent(ToolMouseMoveEvent *event)
{
    mousePosition = event->pos().toPoint();

    if (!mouseTrail.isEmpty())
        mouseTrail.push_back(mousePosition);
    preview();
}

void RubberTool::mousePressEvent(ToolMouseEvent *event)
{
    switch (event->button()) {
    case Qt::LeftButton:
        mouseTrail.push_back(event->pos().toPoint());
        preview();
        event->accept();
        break;
    default:
        break;
    }
}

void RubberTool::mouseReleaseEvent(ToolMouseEvent *event)
{
    switch (event->button()) {
    case Qt::LeftButton:
        commit();
        mouseTrail.clear();
        event->accept();
        break;
    case Qt::RightButton:
        mouseTrail.clear();
        event->accept();
        break;
    default:
        break;
    }
}

void RubberTool::paint(Painter *painter, bool preview)
{
    // Save original painter mode.
    QPainter::CompositionMode originalMode = painter->compositionMode();
    painter->setCompositionMode(QPainter::CompositionMode_Clear);

    // Erase points along mouse trail.
    for (QPoint &position : mouseTrail)
        painter->drawPoint(position);

    // Draw rubber preview (if required).
    if (preview)
        painter->drawPoint(mousePosition);

    // Restore initial painter mode.
    painter->setCompositionMode(originalMode);
}
