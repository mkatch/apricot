#include "toolactionarea.hpp"

#include "tool.hpp"
#include "toolevents.hpp"

/*!
 * \class ToolActionArea
 * \inmodule view
 *
 * \brief A widget for displaying and acting upon an animation frame.
 *
 * This widget extends AnimationFrameView with the possibility of performing actions on the
 * displayed frame using tools. A tool is any specialization of Tool class. Once a tool is attached
 * to ToolActionArea, it receives mouse and keyboard events.
 */

// Properties

/*!
 * \property ToolActionArea::tool
 * \brief The active tool.
 *
 * This property may be `nullptr` meaning that the tool is not set and ToolActionArea is inactive.
 */

// Methods

/*!
 * \brief Constructs the widget with parent widget \a parent.
 */
ToolActionArea::ToolActionArea(QWidget *parent) :
    AnimationFrameView(parent),
    m_tool(nullptr)
{
    this->setMouseTracking(true);
}

void ToolActionArea::setTool(Tool *tool)
{
    if (m_tool == tool)
        return;

    if (m_tool != nullptr)
        m_tool->setActionArea(nullptr);
    m_tool = tool;
    m_tool->setActionArea(this);
    emit toolChanged();
    update();
}

/*!
 * \brief Handles mouse press event \a event.
 *
 * This results in invoking Tool::mousePressEvent() of the active tool.
 */
void ToolActionArea::mousePressEvent(QMouseEvent *event)
{
    if (tool() == nullptr)
        return event->ignore();

    ToolMouseEvent toolEvent(
        event,
        mapToFrame(event->localPos()),
        event->localPos(),
        event->button(),
        event->buttons(),
        event->modifiers()
    );
    tool()->mousePressEvent(&toolEvent);
    if (event->isAccepted())
        lastMousePos = event->localPos();
}

/*!
 * \brief Handles mouse release event \a event.
 *
 * This results in invoking Tool::mouseReleaseEvent() of the active tool.
 */
void ToolActionArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (tool() == nullptr)
        return event->ignore();

    ToolMouseEvent toolEvent(
        event,
        mapToFrame(event->localPos()),
        event->localPos(),
        event->button(),
        event->buttons(),
        event->modifiers()
    );
    tool()->mouseReleaseEvent(&toolEvent);
}

/*!
 * \brief Handles double click event \a event.
 *
 * This results in invoking Tool::mouseDoubleClickEvent() of the active tool.
 */
void ToolActionArea::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (tool() == nullptr)
        return event->ignore();

    ToolMouseEvent toolEvent(
        event,
        mapToFrame(event->localPos()),
        event->localPos(),
        event->button(),
        event->buttons(),
        event->modifiers()
    );
    tool()->mouseDoubleClickEvent(&toolEvent);
}

/*!
 * \brief Handles mouse move event \a event.
 *
 * This results in invoking Tool::mouseMoveEvent() of the active tool.
 */
void ToolActionArea::mouseMoveEvent(QMouseEvent *event)
{
    if (tool() == nullptr)
        return event->ignore();

    ToolMouseMoveEvent toolEvent(
        event,
        mapToFrame(event->localPos()),
        mapToFrame(lastMousePos),
        event->localPos(),
        lastMousePos,
        event->button(),
        event->buttons(),
        event->modifiers()
    );
    tool()->mouseMoveEvent(&toolEvent);
    lastMousePos = event->localPos();
}

/*!
 * \brief Handles mouse wheel event \a event.
 *
 * This results in invoking Tool::wheelEvent() of the active tool.
 */
void ToolActionArea::wheelEvent(QWheelEvent *event)
{
    if (tool() == nullptr)
        return event->ignore();

    ToolWheelEvent toolEvent(
        event,
        mapToFrame(event->posF()),
        event->posF(),
        event->pixelDelta(),
        event->angleDelta(),
        event->buttons(),
        event->modifiers()
    );
    tool()->wheelEvent(&toolEvent);
}

/*!
 * \brief Handles key press event \a event.
 *
 * This results in invoking Tool::keyPressEvent() of the active tool.
 */
void ToolActionArea::keyPressEvent(QKeyEvent *event)
{
    if (tool() == nullptr)
        return event->ignore();

    ToolKeyEvent toolEvent(
        event,
        event->key(),
        event->modifiers()
    );
    tool()->keyPressEvent(&toolEvent);
}

/*!
 * \brief Handles key release event \a event.
 *
 * This results in invoking Tool::keyReleaseEvent() of the active tool.
 */
void ToolActionArea::keyReleaseEvent(QKeyEvent *event)
{
    if (tool() == nullptr)
        return event->ignore();

    ToolKeyEvent toolEvent(
        event,
        event->key(),
        event->modifiers()
    );
    tool()->keyReleaseEvent(&toolEvent);
}
