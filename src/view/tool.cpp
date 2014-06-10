#include "tool.hpp"

/*!
 * \class Tool
 * \inmodule view
 *
 * \brief Base for all tools acting upon edited image.
 *
 * When a Tool is bound to an AnimationFrameView it begis to receive mouse and keyboard events. One
 * canoverride the event handling methods to provide custom behaviour and different actions for the
 * user to invoke on the edited image.
 */

// Properties

/*!
 * \property Tool::view
 * \brief The bound AnimationFrameView.
 *
 * This property can be \c nullptr indicating that the tool is not bound to any AnimationFrameView.
 */

/*!
 * \property Tool::active
 * \brief The boolean value indicating wheather the tool is bound to any AnimationFrameView.
 */

// Signals

/*!
 * \fn Tool::deactivating()
 * \brief Emitted just before the tool is detached from an AnimationFrameView.
 */

// Methods

/*!
 * \brief Construct a tool with parent object \a parent.
 */
Tool::Tool(QObject *parent) :
    QObject(parent),
    m_view(nullptr)
{
    // Do nothing
}

/*!
 * \brief Called just after the tool is attached to an AnimationFrameView.
 *
 * This method can be overriden to perform some initialization if necessary.
 */
void Tool::onActivated()
{
    // Do nothing
}

/*!
 * \brief Called just before the tool is detached from an AnimationFrameView.
 *
 * This method can be overriden to perform some finalizing steps if necessary.
 *
 * \sa deactivating()
 */
void Tool::onDeactivating()
{
    // Do nothing
}

/*!
 * \brief Handles mouse press event \a event.
 *
 * This method can be overriden to perform custom actions on mouse button presses.
 */
void Tool::mousePressEvent(ToolMouseEvent *event)
{
    event->ignore();
}

/*!
 * \brief Handles mouse release event \a event.
 *
 * This method can be overriden to perform custom actions on mouse button releases.
 */
void Tool::mouseReleaseEvent(ToolMouseEvent *event)
{
    event->ignore();
}

/*!
 * \brief Handles double click event \a event.
 *
 * This method can be overriden to perform custom actions on double clicks.
 */
void Tool::mouseDoubleClickEvent(ToolMouseEvent *event)
{
    event->ignore();
}

/*!
 * \brief Handles mouse move event \a event.
 *
 * This method can be overriden to perform custom actions when the mouse moves.
 */
void Tool::mouseMoveEvent(ToolMouseMoveEvent *event)
{
    event->ignore();
}

/*!
 * \brief Handles wheel event \a event.
 *
 * This method can be overriden to perform custom actions when the mouse wheel changes position.
 */
void Tool::wheelEvent(ToolWheelEvent *event)
{
    event->ignore();
}

/*!
 * \brief Handles key press event \a event.
 *
 * This method can be overriden to perform custom actions when a key is pressed.
 */
void Tool::keyPressEvent(ToolKeyEvent *event)
{
    event->ignore();
}

/*!
 * \brief Handles key release event \a event.
 *
 * This method can be overriden to perform custom actions when a key is released.
 */
void Tool::keyReleaseEvent(ToolKeyEvent *event)
{
    event->ignore();
}

/*!
 * \brief Schedules a preview paint.
 *
 * The derived class can call this method to signal that the preview needs to be repainted. In
 * effect, paint() will be called with \e preview set to \c true.
 */
void Tool::preview()
{
    if (!isActive()) {
        qWarning("Tool::preview() was invoked by an inactive tool");
        return;
    }

    view()->toolPreview();
}

/*!
 * \brief Schedules a commiting paint.
 *
 * The derived class can call this method to signal that changes have to be applied to the edited
 * canvas. In effect, paint() will be called with \e preview set to \c false.
 */
void Tool::commit()
{
    if (!isActive()) {
        qWarning("Tool::commit() was invoked by an inactive tool");
        return;
    }

    view()->toolCommit();
}

/*!
 * \brief Paints on the edited canvas using \a painter.
 *
 * This method can be overriden to implement tools which modify the active canvas. The \a preview
 * parameter determines wheather this is a \e {preview paint} or a \e {commiting paint}.
 *
 * Changes made during preview paint are not persistent. They vanish with the next call to paint().
 * This allows for displaying the effect of draw operations provided by the tool, without really
 * affecting the image.
 *
 * If the call is commiting, all changes are applied permanently to the edited canvas.
 */
void Tool::paint(Painter *painter, bool preview)
{
    Q_UNUSED(painter)
    Q_UNUSED(preview)
}

void Tool::setView(AnimationFrameView *view)
{
    if (m_view == view)
        return;

    bool wasNull = (m_view == nullptr);
    m_view = view;
    emit viewChanged();

    if (wasNull) {
        onActivated();
        emit activeChanged();
    } else if (view == nullptr) {
        emit deactivating();
        onDeactivating();
        emit activeChanged();
    }
}
