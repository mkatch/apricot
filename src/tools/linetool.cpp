#include "linetool.hpp"

#include <QtMath>

LineTool::LineTool(QObject *parent) :
    Tool(parent), dragged(false)
{
    // Do nothing.
}

void LineTool::mouseMoveEvent(ToolMouseMoveEvent *event)
{
    mousePosition = event->pos().toPoint();

    if (dragged)
        preview();
}

void LineTool::mouseReleaseEvent(ToolMouseEvent *event)
{
    switch (event->button()) {
    case Qt::LeftButton:
        if (dragged)
            commit();
        else
            dragged = true;
        previousPosition = event->pos().toPoint();
        break;
    case Qt::RightButton:
        dragged = false;
        preview();
        break;
    default:
        break;
    }

    event->accept();
}

void LineTool::keyPressEvent(ToolKeyEvent *event)
{
    keyEvent(event);
}

void LineTool::keyReleaseEvent(ToolKeyEvent *event)
{
    keyEvent(event);
}

void LineTool::keyEvent(ToolKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Control:
        dragModifiers ^= Modifier_Snap;
        preview();
        break;
    default:
        break;
    }
}

void LineTool::paint(Painter *painter, bool preview)
{
    Q_UNUSED(preview)

    if (!dragged)
        return;

    if (dragModifiers & Modifier_Snap)
        painter->drawLine(previousPosition, snapedPosition());
    else
        painter->drawLine(previousPosition, mousePosition);
}

const int LineTool::SNAPPING_LINES = 4;

QPoint LineTool::snapedPosition()
{
    int dx = mousePosition.x() - previousPosition.x();
    int dy = mousePosition.y() - previousPosition.y();

    qreal pi = 4.0 * qAtan(1.0);
    qreal alfa = qAtan2(dy, dx);
    qreal rounded = qRound(SNAPPING_LINES * (alfa / pi));
    qreal beta = rounded * pi / SNAPPING_LINES;

    qreal dist = qSqrt(dx * dx + dy * dy);
    int x = qRound(dist * qCos(beta));
    int y = qRound(dist * qSin(beta));

    return QPoint(previousPosition.x() + x, previousPosition.y() + y);
}
