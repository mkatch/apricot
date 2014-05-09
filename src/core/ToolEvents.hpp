#ifndef CORE_TOOL_EVENTS_HPP
#define CORE_TOOL_EVENTS_HPP


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
        const QPoint &canvasPos,
        const QPointF &areaPos,
        Qt::MouseButton button,
        Qt::MouseButtons buttons,
        Qt::KeyboardModifiers modifiers
    ) :
        ToolEvent(event, modifiers),
        m_canvasPos(canvasPos),
        m_areaPos(areaPos),
        m_button(button),
        m_buttons(buttons)
    {
        // Do nothing
    }

    int x() const { return pos().x(); }
    int y() const { return pos().y(); }
    const QPoint &pos() const { return canvasPos(); }

    int canvasX() const { return canvasPos().x(); }
    int canvasY() const { return canvasPos().y(); }
    const QPoint &canvasPos() const { return m_canvasPos; }

    qreal areaX() const { return areaPos().x(); }
    qreal areaY() const { return areaPos().y(); }
    const QPointF &areaPos() const { return m_areaPos; }

    Qt::MouseButton button() const { return m_button; }
    Qt::MouseButtons buttons() const { return m_buttons; }

private:
    QPoint m_canvasPos;
    QPointF m_areaPos;
    Qt::MouseButton m_button;
    Qt::MouseButtons m_buttons;
};

class ToolMouseMoveEvent : public ToolMouseEvent
{
public:
    ToolMouseMoveEvent(
        QEvent *event,
        const QPoint &canvasPos,
        const QPoint &prevCanvasPos,
        const QPointF &areaPos,
        const QPointF &prevAreaPos,
        Qt::MouseButton button,
        Qt::MouseButtons buttons,
        Qt::KeyboardModifiers modifiers
    ) :
        ToolMouseEvent(event, canvasPos, areaPos, button, buttons, modifiers),
        m_prevCanvasPos(prevCanvasPos),
        m_prevAreaPos(prevAreaPos)
    {
        // Do nothing
    }

    int prevX() const { return prevPos().x(); }
    int prevY() const { return prevPos().y(); }
    const QPoint &prevPos() const { return prevCanvasPos(); }

    int dX() const { return x() - prevX(); }
    int dY() const { return y() - prevY(); }
    QPoint dPos() const { return pos() - prevPos(); }

    int prevCanvasX() const { return prevCanvasPos().x(); }
    int prevCanvasY() const { return prevCanvasPos().y(); }
    const QPoint &prevCanvasPos() const { return m_prevCanvasPos; }

    int dCanvasX() const { return dCanvasPos().x(); }
    int dCanvasY() const { return dCanvasPos().y(); }
    QPoint dCanvasPos() const { return canvasPos() - prevCanvasPos(); }

    qreal prevAreaX() const { return prevAreaPos().x(); }
    qreal prevAreaY() const { return prevAreaPos().y(); }
    const QPointF &prevAreaPos() const { return m_prevAreaPos; }

    qreal dAreaX() const { return areaX() - prevAreaX(); }
    qreal dAreaY() const { return areaY() - prevAreaY(); }
    QPointF dAreaPos() const { return areaPos() - prevAreaPos(); }

private:
    QPoint m_prevCanvasPos;
    QPointF m_prevAreaPos;
};

class ToolMouseWheelEvent : public ToolMouseEvent
{
public:
    ToolMouseWheelEvent(
        QEvent *event,
        const QPoint& canvasPos,
        const QPointF& areaPos,
        const QPoint& pixelDelta,
        const QPoint& angleDelta,
        Qt::MouseButtons buttons,
        Qt::KeyboardModifiers modifiers
    ) :
        ToolMouseEvent(event, canvasPos, areaPos, Qt::NoButton, buttons, modifiers),
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
