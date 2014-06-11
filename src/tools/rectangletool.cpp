#include "rectangletool.hpp"

#include <ApricotUtils>

RectangleTool::RectangleTool(QObject *parent) :
    Tool(parent), dragModifiers()
{
    // Do nothing.
}

void RectangleTool::mouseMoveEvent(ToolMouseMoveEvent *event)
{
    mousePosition = event->pos().toPoint();

    if (dragged) {
        preview();
    }
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
        preview();
        event->accept();
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

void RectangleTool::keyPressEvent(ToolKeyEvent *event)
{
    keyEvent(event);
}

void RectangleTool::keyReleaseEvent(ToolKeyEvent *event)
{
    keyEvent(event);
}

QRect RectangleTool::selectedRectangle()
{
    int dx = mousePosition.x() - startPosition.x();
    int dy = mousePosition.y() - startPosition.y();

    if (dragModifiers & RectangleTool::Square) {
        int size = qMin(qAbs(dx), qAbs(dy));
        dx = sgn(dx) * size;
        dy = sgn(dy) * size;
    }

    QRect rect;
    if (dragModifiers & RectangleTool::Center) {
        QPoint tl(startPosition.x() - dx, startPosition.y() - dy);
        QPoint br(startPosition.x() + dx, startPosition.y() + dy);
        rect.setTopLeft(tl);
        rect.setBottomRight(br);
    } else {
        rect.setTopLeft(startPosition);
        rect.setSize(QSize(dx, dy));
    }

    return rect.normalized();
}

void RectangleTool::paint(Painter *painter, bool preview)
{
    Q_UNUSED(preview)

    if (!dragged)
        return;

    painter->drawRect(selectedRectangle());
}

void RectangleTool::keyEvent(ToolKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Shift:
        dragModifiers ^= RectangleTool::Square;
        preview();
        break;
    case Qt::Key_Control:
        dragModifiers ^= RectangleTool::Center;
        preview();
        break;
    default:
        break;
    }
}
