#ifndef VIEW_TOOLACTIONAREA_HPP
#define VIEW_TOOLACTIONAREA_HPP

#include "animationframeview.hpp"

class Tool;

class ToolActionArea : public AnimationFrameView
{
    Q_OBJECT
    Q_PROPERTY(Tool *tool READ tool WRITE setTool NOTIFY toolChanged)

public:
    explicit ToolActionArea(QWidget *parent = nullptr);

    Tool *tool();
    const Tool *tool() const;
    void setTool(Tool *tool);

signals:
    void toolChanged();

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
    bool pressed;
    QPointF lastMousePos;
};

inline Tool *ToolActionArea::tool()
{
    return m_tool;
}

inline const Tool *ToolActionArea::tool() const
{
    return m_tool;
}

#endif // VIEW_TOOLACTIONAREA_HPP
