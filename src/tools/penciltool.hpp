#ifndef TOOL_PENCILTOOL_HPP
#define TOOL_PENCILTOOL_HPP

#include <QObject>

#include "view/tool.hpp"

class PencilTool : public Tool
{
public:
    explicit PencilTool(QObject *parent = nullptr);

protected:
    void mouseMoveEvent(ToolMouseMoveEvent *event) override;
    void mousePressEvent(ToolMouseEvent *event) override;
    void mouseReleaseEvent(ToolMouseEvent *event) override;
    void paint(Painter *painter, bool preview) override;

    bool pressed;
    QPoint mousePosition;
};

#endif // TOOL_PENCILTOOL_HPP
