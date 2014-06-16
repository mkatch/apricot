#ifndef TOOLS_RECTANGLETOOL_HPP
#define TOOLS_RECTANGLETOOL_HPP

#include <QObject>
#include <QFlag>

#include "view/tool.hpp"

class RectangleTool : public Tool
{
    Q_OBJECT

public:
    explicit RectangleTool(QObject *parent = nullptr);

protected:
    void mouseMoveEvent(ToolMouseMoveEvent *event) override;
    void mousePressEvent(ToolMouseEvent *event) override;
    void mouseReleaseEvent(ToolMouseEvent *event) override;
    void paint(Painter *painter, bool preview) override;

    bool dragged;
    QPoint startPosition;
    QRect selectedRect;
};

#endif // TOOLS_RECTANGLETOOL_HPP
