#ifndef TOOLS_DRAG_TOOL_HPP
#define TOOLS_DRAG_TOOL_HPP


#include <ApricotCore.hpp>


class DragTool : public Tool
{
    Q_OBJECT

public:
    explicit DragTool(QObject *parent = nullptr);

protected:
    virtual void mousePressEvent(ToolMouseEvent *event);
    virtual void mouseDragEvent(ToolMouseMoveEvent *event);
    virtual void mouseWheelEvent(ToolMouseWheelEvent *event);
};


#endif // TOOLS_DRAG_TOOL_HPP
