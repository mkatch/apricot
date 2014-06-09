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
    );

    bool isAcepted() const;
    void setAccepted(bool accepted);
    void accept();
    void ignore();

    Qt::KeyboardModifiers modifiers() const;

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
    );

    qreal x() const;
    qreal y() const;
    const QPointF &pos() const;

    qreal canvasX() const;
    qreal canvasY() const;
    const QPointF &canvasPos() const;

    qreal viewX() const;
    qreal viewY() const;
    const QPointF &viewPos() const;

    Qt::MouseButton button() const;
    Qt::MouseButtons buttons() const;

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
    );

    qreal prevX() const;
    qreal prevY() const;
    const QPointF &prevPos() const;

    qreal dX() const;
    qreal dY() const;
    QPointF dPos() const;

    qreal prevCanvasX() const;
    qreal prevCanvasY() const;
    const QPointF &prevCanvasPos() const;

    qreal dCanvasX() const;
    qreal dCanvasY() const;
    QPointF dCanvasPos() const;

    qreal prevViewX() const;
    qreal prevViewY() const;
    const QPointF &prevViewPos() const;

    qreal dViewX() const;
    qreal dViewY() const;
    QPointF dViewPos() const;

private:
    QPointF m_prevCanvasPos;
    QPointF m_prevViewPos;
};

class ToolWheelEvent : public ToolMouseEvent
{
public:
    ToolWheelEvent(
        QEvent *event,
        const QPointF& canvasPos,
        const QPointF& viewPos,
        const QPoint& pixelDelta,
        const QPoint& angleDelta,
        Qt::MouseButtons buttons,
        Qt::KeyboardModifiers modifiers
    );

    const QPoint& pixelDelta() const;
    const QPoint& angleDelta() const;

private:
    QPoint m_pixelDelta;
    QPoint m_angleDelta;
};

class ToolKeyEvent : public ToolEvent
{
public:
    ToolKeyEvent(
        QEvent *event,
        int key,
        Qt::KeyboardModifier modifiers
    );

    int key() const;

private:
    int m_key;
};

inline ToolEvent::ToolEvent(
    QEvent *event,
    Qt::KeyboardModifiers modifiers
) :
    m_event(event),
    m_modifiers(modifiers)
{
    // Do nothing
}

inline bool ToolEvent::isAcepted() const
{
    return m_event->isAccepted();
}

inline void ToolEvent::setAccepted(bool accepted)
{
    m_event->setAccepted(accepted);
}

inline void ToolEvent::accept()
{
    m_event->accept();
}

inline void ToolEvent::ignore()
{
    m_event->ignore();
}

inline Qt::KeyboardModifiers ToolEvent::modifiers() const
{
    return m_modifiers;
}

inline ToolMouseEvent::ToolMouseEvent(
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

inline qreal ToolMouseEvent::x() const
{
    return pos().x();
}

inline qreal ToolMouseEvent::y() const
{
    return pos().y();
}

inline const ToolMouseEvent::QPointF &pos() const
{
    return canvasPos();
}

inline qreal ToolMouseEvent::canvasX() const
{
    return canvasPos().x();
}

inline qreal ToolMouseEvent::canvasY() const
{
    return canvasPos().y();
}

inline const ToolMouseEvent::QPointF &canvasPos() const
{
    return m_canvasPos;
}

inline qreal ToolMouseEvent::viewX() const
{
    return viewPos().x();
}

inline qreal ToolMouseEvent::viewY() const
{
    return viewPos().y();
}

inline const QPointF &ToolMouseEvent::viewPos() const
{
    return m_viewPos;
}

inline Qt::MouseButton ToolMouseEvent::button() const
{
    return m_button;
}

inline Qt::MouseButtons ToolMouseEvent::buttons() const
{
    return m_buttons;
}

inline ToolMouseMoveEvent::ToolMouseMoveEvent(
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

inline qreal ToolMouseMoveEvent::prevX() const
{
	return prevPos().x();
}

inline qreal ToolMouseMoveEvent::prevX() const
{
	return prevPos().y();
}

inline const QPointF &ToolMouseMoveEvent::prevPos() const
{
	return prevCanvasPos();
}

inline qreal ToolMouseMoveEvent::prevX() const
{
	return x() - prevX();
}

inline qreal ToolMouseMoveEvent::prevX() const
{
	return y() - prevY();
}

inline qreal ToolMouseMoveEvent::prevX() const
{
	return pos() - prevPos();
}

inline qreal ToolMouseMoveEvent::prevCanvasX() const
{
	return prevCanvasPos().x();
}

inline qreal ToolMouseMoveEvent::prevCanvasY() const
{
	return prevCanvasPos().y();
}

inline const QPointF &prevCanvasPos() const
{
	return m_prevCanvasPos;
}

inline qreal ToolMouseMoveEvent::dCanvasX() const
{
	return dCanvasPos().x();
}

inline qreal ToolMouseMoveEvent::dCanvasY() const
{
	return dCanvasPos().y();
}

inline QPointF ToolMouseMoveEvent::dCanvasPos() const
{
	return canvasPos() - prevCanvasPos();
}

inline qreal ToolMouseMoveEvent::prevViewX() const
{
	return prevViewPos().x();
}

inline qreal ToolMouseMoveEvent::prevViewY() const
{
	return prevViewPos().y();
}

inline const QPointF &ToolMouseMoveEvent::prevViewPos() const
{
	return m_prevViewPos;
}

inline qreal ToolMouseMoveEvent::dViewX() const
{
	return viewX() - prevViewX();
}

inline qreal ToolMouseMoveEvent::dViewY() const
{
	return viewY() - prevViewY();
}

inline QPointF ToolMouseMoveEvent::dViewPos() const
{
	return viewPos() - prevViewPos();
}

ToolWheelEvent::ToolWheelEvent(
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

const QPoint& ToolWheelEvent::pixelDelta() const
{
    return m_pixelDelta;
}

const QPoint& ToolWheelEvent::angleDelta() const
{
    return m_angleDelta;
}

inline ToolKeyEvent::ToolKeyEvent(
    QEvent *event,
    int key,
    Qt::KeyboardModifier modifiers
) :
    ToolEvent(event, modifiers),
    m_key(key)
{
    // Do nothing
}

inline ToolKeyEvent::key() const
{
    return m_key;
}

#endif // CORE_TOOL_EVENTS_HPP
