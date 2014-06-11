#ifndef TOOL_LINETOOL_HPP
#define TOOL_LINETOOL_HPP

#include <QObject>
#include <QFlag>

#include <ApricotView>

class LineTool : public Tool
{
    Q_OBJECT

public:
    explicit LineTool(QObject *parent = nullptr);

protected:
    void mouseMoveEvent(ToolMouseMoveEvent *event) override;
    void mouseReleaseEvent(ToolMouseEvent *event) override;
    void keyPressEvent(ToolKeyEvent *event) override;
    void keyEvent(ToolKeyEvent *event);
    void keyReleaseEvent(ToolKeyEvent *event) override;
    void paint(Painter *painter, bool preview) override;

private:
    static const int SNAPPING_LINES;

    enum DragModifier {
        Modifier_None = 0x0,
        Modifier_Snap = 0x1
    };
    Q_DECLARE_FLAGS(DragModifiers, DragModifier)

    bool dragged;
    DragModifiers dragModifiers;

    QPoint mousePosition;
    QPoint previousPosition;
    QPoint snapedPosition();
};

#endif // TOOL_LINETOOL_HPP
