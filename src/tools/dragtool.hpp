#ifndef TOOLS_DRAGTOOL_HPP
#define TOOLS_DRAGTOOL_HPP

#include <ApricotView>

class DragTool : public Tool
{
    Q_OBJECT

public:
    explicit DragTool(QObject *parent = nullptr);

protected:
    virtual void mouseMoveEvent(ToolMouseMoveEvent *event) override;
    virtual void wheelEvent(ToolWheelEvent *event) override;
};

#endif // TOOLS_DRAGTOOL_HPP
