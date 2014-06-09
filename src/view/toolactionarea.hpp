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
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    Tool *m_tool;
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
