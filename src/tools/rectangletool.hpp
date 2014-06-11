#ifndef TOOLS_RECTANGLETOOL_HPP
#define TOOLS_RECTANGLETOOL_HPP

#include <QObject>
#include <QFlag>

#include <ApricotView>

class RectangleTool : public Tool
{
    Q_OBJECT

public:
    explicit RectangleTool(QObject *parent = nullptr);

protected:
    void mouseMoveEvent(ToolMouseMoveEvent *event) override;
    void mousePressEvent(ToolMouseEvent *event) override;
    void mouseReleaseEvent(ToolMouseEvent *event) override;
    void keyPressEvent(ToolKeyEvent *event) override;
    void keyReleaseEvent(ToolKeyEvent *event) override;
    void paint(Painter *painter, bool preview) override;

    QRect selectedRectangle();

private:
    void keyEvent(ToolKeyEvent *event);

    enum DragModifier {
        Normal = 0x0,
        Square = 0x1,
        Center = 0x2
    };
    Q_DECLARE_FLAGS(DragModifiers, DragModifier)

    bool dragged;
    DragModifiers dragModifiers;
    QPoint mousePosition;
    QPoint startPosition;
};

#endif // TOOLS_RECTANGLETOOL_HPP
