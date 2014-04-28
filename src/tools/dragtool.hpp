#ifndef DRAGTOOL_HPP
#define DRAGTOOL_HPP

#include <ApricotCore.hpp>

class DragTool : public Tool
{
    Q_OBJECT

public:
    explicit DragTool(QObject *parent = 0);

protected:
    virtual void mousePressEvent(ToolMouseEvent* event);

    virtual void mouseDragEvent(ToolMouseMoveEvent* event);

    virtual void mouseWheelEvent(ToolMouseWheelEvent *event);
};

#endif // DRAGTOOL_HPP
