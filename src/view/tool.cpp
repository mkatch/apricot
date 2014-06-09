#include "tool.hpp"

#include "toolactionarea.hpp"

Tool::Tool(QObject *parent) :
    QObject(parent)
{
    // Do nothing
}

void Tool::onActivated()
{
    // Do nothing
}

void Tool::onDeactivating()
{
    // Do nothing
}

void Tool::mousePressEvent(ToolMouseEvent *event)
{
    event->ignore();
}

void Tool::mouseReleaseEvent(ToolMouseEvent *event)
{
    event->ignore();
}

void Tool::mouseDoubleClickEvent(ToolMouseEvent *event)
{
    event->ignore();
}

void Tool::mouseMoveEvent(ToolMouseMoveEvent *event)
{
    event->ignore();
}

void Tool::wheelEvent(ToolWheelEvent *event)
{
    event->ignore();
}

void Tool::keyPressEvent(ToolKeyEvent *event)
{
    event->ignore();
}

void Tool::keyReleaseEvent(ToolKeyEvent *event)
{
    event->ignore();
}

void Tool::setActionArea(ToolActionArea *actionArea)
{
    if (m_actionArea == actionArea)
        return;

    bool wasNull = (m_actionArea == nullptr);
    m_actionArea = actionArea;
    emit actionAreaChanged();

    if (wasNull) {
        onActivated();
        emit activeChanged();
    } else if (actionArea == nullptr) {
        emit deactivating();
        onDeactivating();
        emit activeChanged();
    }
}
