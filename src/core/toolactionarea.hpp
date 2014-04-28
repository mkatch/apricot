#ifndef TOOLACTIONAREA_HPP
#define TOOLACTIONAREA_HPP

#include <QQuickItem>

#include "canvasview.hpp"

class Tool;

class ToolActionArea : public CanvasView
{
    Q_OBJECT
    Q_PROPERTY(Tool *tool READ tool WRITE setTool NOTIFY toolChanged)
    Q_PROPERTY(bool active READ active NOTIFY activeChanged)

public:
    explicit ToolActionArea(QQuickItem *parent = 0);

    Tool *tool() const { return m_tool; }
    void setTool(Tool *tool);

    bool active() const { return tool() != 0; }

    QPointF canvasToArea(const QPoint& canvasPos) const;

    QPoint areaToCanvas(const QPointF& areaPos) const;

signals:
    void toolChanged();
    void activeChanged();

public slots:

protected:
    virtual void mousePressEvent(QMouseEvent *event);

    virtual void mouseReleaseEvent(QMouseEvent *event);

    virtual void mouseDoubleClickEvent(QMouseEvent *event);

    virtual void mouseMoveEvent(QMouseEvent *event);

    virtual void hoverMoveEvent(QHoverEvent *event);

    virtual void wheelEvent(QWheelEvent *event);

    virtual QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data);

private:
    Tool *m_tool;

    bool toolChangedSinceLastUpdatePaintNode;
    QPointF lastMousePos;

    void applyToolToCanvas(bool preview);
};

#endif // TOOLACTIONAREA_HPP
