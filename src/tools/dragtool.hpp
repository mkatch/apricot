#ifndef TOOLS_DRAG_TOOL_HPP
#define TOOLS_DRAG_TOOL_HPP


#include <ApricotCore>


class DragTool : public Tool
{
    Q_OBJECT

public:
    explicit DragTool(QObject *parent = nullptr);

protected:
    virtual void mousePressEvent(ToolMouseEvent *event) override;
    virtual void mouseDragEvent(ToolMouseMoveEvent *event) override;
    virtual void mouseWheelEvent(ToolMouseWheelEvent *event) override;
};


#endif // TOOLS_DRAG_TOOL_HPP
