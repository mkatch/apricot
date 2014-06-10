#ifndef VIEW_TOOL_HPP
#define VIEW_TOOL_HPP

#include <QObject>

#include "animationframeview.hpp"
#include "toolevents.hpp"

class Tool : public QObject
{
    Q_OBJECT
    Q_PROPERTY(AnimationFrameView *view READ view NOTIFY viewChanged)
    Q_PROPERTY(bool active READ isActive NOTIFY activeChanged)

public:
    explicit Tool(QObject *parent = nullptr);

    AnimationFrameView *view();
    const AnimationFrameView *view() const;

    bool isActive() const;

signals:
    void viewChanged();
    void activeChanged();
    void deactivating();

protected:
    virtual void onActivated();
    virtual void onDeactivating();

    virtual void mousePressEvent(ToolMouseEvent *event);
    virtual void mouseReleaseEvent(ToolMouseEvent *event);
    virtual void mouseDoubleClickEvent(ToolMouseEvent *event);
    virtual void mouseMoveEvent(ToolMouseMoveEvent *event);
    virtual void wheelEvent(ToolWheelEvent *event);
    virtual void keyPressEvent(ToolKeyEvent *event);
    virtual void keyReleaseEvent(ToolKeyEvent *event);

    void preview();
    void commit();
    virtual void paint(Painter *painter, bool preview);

private:
    AnimationFrameView *m_view;

    void setView(AnimationFrameView *view);

    friend class AnimationFrameView;
};

inline AnimationFrameView *Tool::view()
{
    return m_view;
}

inline const AnimationFrameView *Tool::view() const
{
    return m_view;
}

inline bool Tool::isActive() const
{
    return view() != nullptr;
}

#endif // VIEW_TOOL_HPP
