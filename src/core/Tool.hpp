#ifndef TOOL_HPP
#define TOOL_HPP

#include <QObject>
#include <QtQuick>

class ToolActionArea;
class ToolMouseEvent;
class ToolMouseMoveEvent;
class ToolMouseWheelEvent;

class Tool : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ToolActionArea *actionArea READ actionArea NOTIFY actionAreaChanged)
    Q_PROPERTY(bool active READ active NOTIFY activeChanged)

public:
    explicit Tool(QObject *parent = nullptr);

    ToolActionArea *actionArea() const { return m_actionArea; }

    bool active() const { return m_actionArea != nullptr; }

signals:
    void actionAreaChanged();
    void activeChanged();
    void activated();
    void deactivating();
    void deactivated();

public slots:

protected:
    virtual void onActivated();

    virtual void onDeactivating();

    virtual void mousePressEvent(ToolMouseEvent *event);

    virtual void mouseReleaseEvent(ToolMouseEvent *event);

    virtual void mouseDoubleClickEvent(ToolMouseEvent *event);

    virtual void mouseHoverEvent(ToolMouseMoveEvent *event);

    virtual void mouseDragEvent(ToolMouseMoveEvent *event);

    virtual void mouseWheelEvent(ToolMouseWheelEvent *event);

    virtual QSGNode *updateActionAreaPaintNode(QSGNode *oldNode);

private:
    ToolActionArea* m_actionArea;

    void setActionArea(ToolActionArea *actionArea);

    friend class ToolActionArea;
};

#endif // TOOL_HPP
