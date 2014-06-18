#ifndef TOOLS_DRAGTOOL_HPP
#define TOOLS_DRAGTOOL_HPP

#include "view/tool.hpp"

class DragTool : public Tool
{
    Q_OBJECT

public:
    explicit DragTool(QObject *parent = nullptr);

protected:
    void mouseMoveEvent(ToolMouseMoveEvent *event) override;
    void wheelEvent(ToolWheelEvent *event) override;

private:
    QPoint lastPoint;
};

#endif // TOOLS_DRAGTOOL_HPP
