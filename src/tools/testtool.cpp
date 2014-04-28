#include "testtool.hpp"

#include <QDebug>

TestTool::TestTool() :
    pos(0, 0)
{
    // Do nothing
}

void TestTool::mouseHoverEvent(ToolMouseMoveEvent *event)
{
    pos = event->pos();
    actionArea()->dragTo(event->pos());
}

void TestTool::mouseWheelEvent(ToolMouseWheelEvent *event)
{
    actionArea()->setZoom(actionArea()->zoom() * (1.0 + event->angleDelta().y() / 1200.0));
}

QSGNode *TestTool::updateActionAreaPaintNode(QSGNode *oldNode)
{
    QSGSimpleRectNode *n = static_cast<QSGSimpleRectNode *>(oldNode);
    if (!n) {
        n = new QSGSimpleRectNode();
        n->setColor(Qt::red);
    }
    n->setRect(pos.x(), pos.y(), 10, 10);
    return n;
}
