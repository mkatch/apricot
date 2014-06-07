#ifndef VIEW_TOOLEVENTS_HPP
#define VIEW_TOOLEVENTS_HPP

#include <QEvent>
#include <QInputEvent>
#include <QMouseEvent>

class ToolEvent
{
public:
    ToolEvent(
        QEvent *event,
        Qt::KeyboardModifiers modifiers
    ) :
        m_event(event),
        m_modifiers(modifiers)
    {
        // Do nothing
    }

    bool accepted() const { return m_event->isAccepted(); }
    void setAccepted(bool accepted) { m_event->setAccepted(accepted); }
    void accept() { m_event->accept(); }
    void ignore() { m_event->ignore(); }

    Qt::KeyboardModifiers modifiers() { return m_modifiers; }

private:
    QEvent *m_event;
    Qt::KeyboardModifiers m_modifiers;
};

class ToolMouseEvent : public ToolEvent
{
public:
    ToolMouseEvent(
        QEvent *event,
        const QPointF &canvasPos,
        const QPointF &viewPos,
        Qt::MouseButton button,
        Qt::MouseButtons buttons,
        Qt::KeyboardModifiers modifiers
    ) :
        ToolEvent(event, modifiers),
        m_canvasPos(canvasPos),
        m_viewPos(viewPos),
        m_button(button),
        m_buttons(buttons)
    {
        // Do nothing
    }

    qreal x() const { return pos().x(); }
    qreal y() const { return pos().y(); }
    const QPointF &pos() const { return canvasPos(); }

    qreal canvasX() const { return canvasPos().x(); }
    qreal canvasY() const { return canvasPos().y(); }
    const QPointF &canvasPos() const { return m_canvasPos; }

    qreal viewX() const { return viewPos().x(); }
    qreal viewY() const { return viewPos().y(); }
    const QPointF &viewPos() const { return m_viewPos; }

    Qt::MouseButton button() const { return m_button; }
    Qt::MouseButtons buttons() const { return m_buttons; }

private:
    QPointF m_canvasPos;
    QPointF m_viewPos;
    Qt::MouseButton m_button;
    Qt::MouseButtons m_buttons;
};

class ToolMouseMoveEvent : public ToolMouseEvent
{
public:
    ToolMouseMoveEvent(
        QEvent *event,
        const QPointF &canvasPos,
        const QPointF &prevCanvasPos,
        const QPointF &viewPos,
        const QPointF &prevAreaPos,
        Qt::MouseButton button,
        Qt::MouseButtons buttons,
        Qt::KeyboardModifiers modifiers
    ) :
        ToolMouseEvent(event, canvasPos, viewPos, button, buttons, modifiers),
        m_prevCanvasPos(prevCanvasPos),
        m_prevViewPos(prevAreaPos)
    {
        // Do nothing
    }

    qreal prevX() const { return prevPos().x(); }
    qreal prevY() const { return prevPos().y(); }
    const QPointF &prevPos() const { return prevCanvasPos(); }

    qreal dX() const { return x() - prevX(); }
    qreal dY() const { return y() - prevY(); }
    QPointF dPos() const { return pos() - prevPos(); }

    qreal prevCanvasX() const { return prevCanvasPos().x(); }
    qreal prevCanvasY() const { return prevCanvasPos().y(); }
    const QPointF &prevCanvasPos() const { return m_prevCanvasPos; }

    qreal dCanvasX() const { return dCanvasPos().x(); }
    qreal dCanvasY() const { return dCanvasPos().y(); }
    QPointF dCanvasPos() const { return canvasPos() - prevCanvasPos(); }

    qreal prevViewX() const { return prevViewPos().x(); }
    qreal prevViewY() const { return prevViewPos().y(); }
    const QPointF &prevViewPos() const { return m_prevViewPos; }

    qreal dViewX() const { return viewX() - prevViewX(); }
    qreal dViewY() const { return viewY() - prevViewY(); }
    QPointF dViewPos() const { return viewPos() - prevViewPos(); }

private:
    QPointF m_prevCanvasPos;
    QPointF m_prevViewPos;
};

class ToolMouseWheelEvent : public ToolMouseEvent
{
public:
    ToolMouseWheelEvent(
        QEvent *event,
        const QPointF& canvasPos,
        const QPointF& viewPos,
        const QPoint& pixelDelta,
        const QPoint& angleDelta,
        Qt::MouseButtons buttons,
        Qt::KeyboardModifiers modifiers
    ) :
        ToolMouseEvent(event, canvasPos, viewPos, Qt::NoButton, buttons, modifiers),
        m_pixelDelta(pixelDelta),
        m_angleDelta(angleDelta)
    {
        // Do nothing
    }

    const QPoint& pixelDelta() const { return m_pixelDelta; }
    const QPoint& angleDelta() const { return m_angleDelta; }

private:
    QPoint m_pixelDelta;
    QPoint m_angleDelta;
};


#endif // CORE_TOOL_EVENTS_HPP
