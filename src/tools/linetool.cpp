#include "linetool.hpp"

#include <QtMath>

const int LineTool::SNAPPING_LINES_COUNT = 4;

LineTool::LineTool(QObject *parent) :
    Tool(parent), dragged(false)
{
    // Do nothing.
}

void LineTool::mouseMoveEvent(ToolMouseMoveEvent *event)
{
    if (!dragged)
        return;

    QPoint mousePosition = event->pos().toPoint();

    if (event->modifiers() & Qt::ControlModifier)
        currentPosition = snappedPosition(mousePosition);
    else
        currentPosition = mousePosition;
    preview();
}

void LineTool::mouseReleaseEvent(ToolMouseEvent *event)
{
    switch (event->button()) {
    case Qt::LeftButton:
        if (dragged) {
            commit();
            previousPosition = currentPosition;
        } else {
            dragged = true;
            previousPosition = event->pos().toPoint();
        }
        event->accept();
        break;
    case Qt::RightButton:
        if (dragged) {
            dragged = false;
            event->accept();
            preview();
        }
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

    painter->drawLine(previousPosition, currentPosition);
}

QPoint LineTool::snappedPosition(QPoint mousePosition)
{
    int dx = mousePosition.x() - previousPosition.x();
    int dy = mousePosition.y() - previousPosition.y();

    qreal pi = 4.0 * qAtan(1.0);
    qreal alfa = qAtan2(dy, dx);
    qreal rounded = qRound(SNAPPING_LINES_COUNT * (alfa / pi));
    qreal beta = rounded * pi / SNAPPING_LINES_COUNT;

    qreal dist = qSqrt(dx * dx + dy * dy);
    int x = qRound(dist * qCos(beta));
    int y = qRound(dist * qSin(beta));

    return QPoint(previousPosition.x() + x, previousPosition.y() + y);
}
