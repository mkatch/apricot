#ifndef TOOL_LINETOOL_HPP
#define TOOL_LINETOOL_HPP

#include <QObject>
#include <QFlag>

#include "view/tool.hpp"

class LineTool : public Tool
{
    Q_OBJECT

public:
    explicit LineTool(QObject *parent = nullptr);

protected:
    void mouseMoveEvent(ToolMouseMoveEvent *event) override;
    void mouseReleaseEvent(ToolMouseEvent *event) override;
    void paint(Painter *painter, bool preview) override;

private:
    static const int SNAPPING_LINES_COUNT;

    bool dragged;
    QPoint previousPosition;
    QPoint currentPosition;
    QPoint snappedPosition(QPoint mousePosition);
};

#endif // TOOL_LINETOOL_HPP
