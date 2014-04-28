#ifndef TESTTOOL_HPP
#define TESTTOOL_HPP

#include <QtCore>
#include "ApricotCore.hpp"

class TestTool : public Tool
{
public:
    TestTool();

protected:
    virtual void mouseHoverEvent(ToolMouseMoveEvent *event);

    virtual void mouseWheelEvent(ToolMouseWheelEvent *event);

    virtual QSGNode *updateActionAreaPaintNode(QSGNode *oldNode);

private:
    QPointF pos;
};

#endif // TESTTOOL_HPP
