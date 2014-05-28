#ifndef CORE_TOOL_ACTION_AREA_HPP
#define CORE_TOOL_ACTION_AREA_HPP


#include "CanvasView.hpp"


class Tool;

class ToolActionArea : public CanvasView
{
    Q_OBJECT
    Q_PROPERTY(Tool *tool READ tool WRITE setTool NOTIFY toolChanged)
    Q_PROPERTY(bool active READ active NOTIFY activeChanged)

public:
    explicit ToolActionArea(QWidget *parent = nullptr);

    Tool *tool() const { return m_tool; }
    void setTool(Tool *tool);

    bool active() const { return tool() != nullptr; }

signals:
    void toolChanged();
    void activeChanged();

public slots:

protected:
    virtual bool event(QEvent* event) override;

    void dispatchMousePressEvent(QMouseEvent *event);

    void dispatchMouseReleaseEvent(QMouseEvent *event);

    void dispatchMouseDoubleClickEvent(QMouseEvent *event);

    void dispatchMouseMoveEvent(QMouseEvent *event);

    void dispatchHoverMoveEvent(QHoverEvent *event);

    void dispatchWheelEvent(QWheelEvent *event);

private:
    Tool *m_tool;

    QPointF lastMousePos;

    void applyToolToCanvas(bool preview);
};


#endif // CORE_TOOL_ACTION_AREA_HPP
