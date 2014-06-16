#include "rectangletool.hpp"

#include <ApricotUtils>

RectangleTool::RectangleTool(QObject *parent) :
    Tool(parent), dragged(false)
{
    // Do nothing.
}

void RectangleTool::mouseMoveEvent(ToolMouseMoveEvent *event)
{
    if (!dragged)
        return;

    QPoint mousePosition = event->pos().toPoint();
    int dx = mousePosition.x() - startPosition.x();
    int dy = mousePosition.y() - startPosition.y();

    if (event->modifiers() & Qt::ShiftModifier) {
        int size = qMin(qAbs(dx), qAbs(dy));
        dx = sgn(dx) * size;
        dy = sgn(dy) * size;
    }

    QRect rect;
    if (event->modifiers() & Qt::ControlModifier) {
        QPoint tl(startPosition.x() - dx, startPosition.y() - dy);
        QPoint br(startPosition.x() + dx, startPosition.y() + dy);
        rect.setTopLeft(tl);
        rect.setBottomRight(br);
    } else {
        rect.setTopLeft(startPosition);
        rect.setSize(QSize(dx, dy));
    }

    selectedRect = rect.normalized();
    preview();
}

void RectangleTool::mousePressEvent(ToolMouseEvent *event)
{
    switch (event->button()) {
    case Qt::LeftButton:
        startPosition = event->pos().toPoint();
        dragged = true;
        event->accept();
        break;
    case Qt::RightButton:
        dragged = false;
        event->accept();
        preview();
        break;
    default:
        break;
    }
}

void RectangleTool::mouseReleaseEvent(ToolMouseEvent *event)
{
    Q_UNUSED(event)

    if (dragged) {
        commit();
        dragged = false;
    }
}

void RectangleTool::paint(Painter *painter, bool preview)
{
    Q_UNUSED(preview)

    if (!dragged)
        return;

    painter->drawRect(selectedRect);
}
