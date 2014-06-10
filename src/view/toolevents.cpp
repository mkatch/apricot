#include "toolevents.hpp"

/*!
 * \class ToolEvent
 * \inmodule view
 *
 * \brief Base for all events delivered to Tool.
 */

/*!
 * \fn ToolEvent::ToolEvent(QEvent *event, Qt::KeyboardModifiers modifiers)
 * \brief Construct the event wrapping around \a event with keyboard modifiers \a modifiers.
 */

/*!
 * \fn ToolEvent::isAcepted() const
 * \brief Returns a boolean value indicating wheather the event was accepted by the Tool it was
 * dispatched upon.
 */

/*!
 * \fn ToolEvent::setAccepted(bool accepted)
 * \brief Sets the boolean value indicating wheather the event was accepted by the Tool it was
 * dispatched upon to \a accepted.
 */

/*!
 * \fn ToolEvent::accept()
 * \brief Equivalent to setAccepted(true).
 */

/*!
 * \fn ToolEvent::ignore()
 * \brief Equivalent to setAccepted(false).
 */

/*!
 * \fn ToolEvent::modifiers() const
 * \brief Returns the modifier keys that were pressed when the event occured.
 */

/*!
 * \class ToolMouseEvent
 * \inmodule view
 *
 * \brief A ToolEvent representing mouse events.
 */

/*!
 * \fn ToolMouseEvent::ToolMouseEvent(
 *     QEvent *event,
 *     const QPointF &canvasPos,
 *     const QPointF &viewPos,
 *     Qt::MouseButton button,
 *     Qt::MouseButtons buttons,
 *     Qt::KeyboardModifiers modifiers
 * )
 *
 * \brief Constructs an event wrapping around \a event with canvas position \a canvasPos, view
 * position \a viewPos, triggering button \a button, pressed buttons \a buttons key modifiers
 * \a modifiers.
 */

/*!
 * \fn ToolMouseEvent::x() const
 * \brief Returns the \e x coordinate of mouse cursor in canvas coordinates.
 *
 * Equivalent to canvasX().
 */

/*!
 * \fn ToolMouseEvent::y() const
 * \brief Returns the \e y coordinate of mouse cursor in canvas coordinates.
 *
 * Equivalent to canvasY().
 */

/*!
 * \fn ToolMouseEvent::pos() const
 * \brief Returns the position of mouse cursor in canvas coordinates.
 *
 * Equivalent to canvasPos().
 */

/*!
 * \fn ToolMouseEvent::canvasX() const
 * \brief Returns the \e x coordinate of mouse cursor in canvas coordinates.
 */

/*!
 * \fn ToolMouseEvent::canvasY() const
 * \brief Returns the \e x coordinate of mouse cursor in canvas coordinates.
 */

/*!
 * \fn ToolMouseEvent::canvasPos() const
 * \brief Returns the position of mouse cursor in canvas coordinates.
 */

/*!
 * \fn ToolMouseEvent::viewX() const
 * \brief Returns the \e x coordinate of mouse cursor in canvas coordinates.
 */

/*!
 * \fn ToolMouseEvent::viewY() const
 * \brief Returns the \e x coordinate of mouse cursor in view coordinates.
 */

/*!
 * \fn ToolMouseEvent::viewPos() const
 * \brief Returns the position of mouse cursor in view coordinates.
 */

/*!
 * \fn ToolMouseEvent::button() const
 * \brief Returns the button that triggered the event.
 *
 * Not all mouse events are related to a button press or release. In those cases, this method
 * returns 0.
 */

/*!
 * \fn ToolMouseEvent::buttons() const
 * \brief Returns button flags of the mouse buttons pressed when the event occured.
 */

/*!
 * \class ToolMouseMoveEvent
 * \inmodule view
 *
 * \brief A ToolEvent representing mouse movement.
 */

/*!
 * \fn ToolMouseMoveEvent::ToolMouseMoveEvent(
 *     QEvent *event,
 *     const QPointF &canvasPos,
 *     const QPointF &prevCanvasPos,
 *     const QPointF &viewPos,
 *     const QPointF &prevViewPos,
 *     Qt::MouseButtons buttons,
 *     Qt::KeyboardModifiers modifiers
 * )
 *
 * \brief Construct an event wrapping around \a event with canvas position \a canvasPos, previous
 * canvas position \a prevCanvasPos, view position \a viewPos, previous view pos \a prevViewPos,
 * pressed mouse butons \a buttons and pressed modifier keys \a modifiers.
 */

/*!
 * \fn ToolMouseMoveEvent::prevX() const
 * \brief Returns the \e x coordinate of mouse cursor from before the move in canvas coordinates.
 *
 * Equivalent to prevCanvasX().
 */

/*!
 * \fn ToolMouseMoveEvent::prevY() const
 * \brief Returns the \e y coordinate of mouse cursor from before the move in canvas coordinates.
 *
 * Equivalent to prevCanvasY().
 */

/*!
 * \fn ToolMouseMoveEvent::prevPos() const
 * \brief Returns the position of mouse cursor from before the move in canvas coordinates.
 *
 * Equivalent to prevCanvasPos().
 */

/*!
 * \fn ToolMouseMoveEvent::dX() const
 * \brief Returns the \e x displacement of the mouse cursor in canvas coordinates.
 *
 * Equivalent to dCanvasX();
 */

/*!
 * \fn ToolMouseMoveEvent::dY() const
 * \brief Returns the \e y displacement of the mouse cursor in canvas coordinates.
 *
 * Equivalent to dCanvasY();
 */

/*!
 * \fn ToolMouseMoveEvent::dPos() const
 * \brief Returns the displacement of the mouse cursor in canvas coordinates.
 *
 * Equivalent to dCanvasPos();
 */

/*!
 * \fn ToolMouseMoveEvent::prevCanvasX() const
 * \brief Returns the \e x coordinate of mouse cursor from before the move in canvas coordinates.
 */

/*!
 * \fn ToolMouseMoveEvent::prevCanvasY() const
 * \brief Returns the \e y coordinate of mouse cursor from before the move in canvas coordinates.
 */

/*!
 * \fn ToolMouseMoveEvent::prevCanvasPos() const
 * \brief Returns the position of mouse cursor from before the move in canvas coordinates.
 */

/*!
 * \fn ToolMouseMoveEvent::dCanvasX() const
 * \brief Returns the \e x displacement of the mouse cursor in canvas coordinates.
 */

/*!
 * \fn ToolMouseMoveEvent::dCanvasY() const
 * \brief Returns the \e y displacement of the mouse cursor in canvas coordinates.
 */

/*!
 * \fn ToolMouseMoveEvent::dCanvasPos() const
 * \brief Returns the displacement of the mouse cursor in canvas coordinates.
 */

/*!
 * \fn ToolMouseMoveEvent::prevViewX() const
 * \brief Returns the \e x coordinate of mouse cursor from before the move in view coordinates.
 */

/*!
 * \fn ToolMouseMoveEvent::prevViewY() const
 * \brief Returns the \e y coordinate of mouse cursor from before the move in view coordinates.
 */

/*!
 * \fn ToolMouseMoveEvent::prevViewPos() const
 * \brief Returns the position of mouse cursor from before the move in view coordinates.
 */

/*!
 * \fn ToolMouseMoveEvent::dViewX() const
 * \brief Returns the \e x displacement of the mouse cursor in view coordinates.
 */

/*!
 * \fn ToolMouseMoveEvent::dViewY() const
 * \brief Returns the \e y displacement of the mouse cursor in view coordinates.
 */

/*!
 * \fn ToolMouseMoveEvent::dViewPos() const
 * \brief Returns the displacement of the mouse cursor in view coordinates.
 */

/*!
 * \class ToolWheelEvent
 * \inmodule view
 *
 * \brief A ToolEvent representing mouse wheel event.
 */

/*!
 * \fn ToolWheelEvent::ToolWheelEvent(
 *     QEvent *event,
 *     const QPointF &canvasPos,
 *     const QPointF &viewPos,
 *     const QPoint &pixelDelta,
 *     const QPoint &angleDelta,
 *     Qt::MouseButtons buttons,
 *     Qt::KeyboardModifiers modifiers
 * )
 *
 * \brief Construct an event wrapping around \a event with canvas position \a canvasPos, view
 * position \a viewPos, pixel wheel displacement \a pixelDelta, angle wheel displacement
 * \a angleDelta, pressed buttons \a buttons and pressed modifier keys \a modifiers.
 */

/*!
 * \fn ToolWheelEvent::pixelDelta() const
 * \brief Returns the displacement of the wheel in pixels.
 */

/*!
 * \fn ToolWheelEvent::angleDelta() const
 * \brief Returns the displacement of the wheel in angle units.
 */

/*!
 * \class ToolKeyEvent
 * \inmodule view
 *
 * \brief A ToolEvent representing key presses and releases.
 */

/*!
 * \fn ToolKeyEvent::ToolKeyEvent(QEvent *event, int key, Qt::KeyboardModifiers modifiers)
 * \brief Constructs an event wrapping around \a event with pressed key \a key and pressed
 * modifier keys \a modifiers.
 */

/*!
 * \fn ToolKeyEvent::key() const
 * \brief Returns the keycode of the key that was pressed or released.
 */
