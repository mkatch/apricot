#ifndef TOOLS_RUBBERTOOL_HPP
#define TOOLS_RUBBERTOOL_HPP

#include <QObject>
#include <QVector>

#include "view/tool.hpp"

class RubberTool : public Tool
{
    Q_OBJECT

public:
    explicit RubberTool(QObject *parent = nullptr);

protected:
    void mouseMoveEvent(ToolMouseMoveEvent *event) override;
    void mousePressEvent(ToolMouseEvent *event) override;
    void mouseReleaseEvent(ToolMouseEvent *event) override;
    void paint(Painter *painter, bool preview) override;

    QPoint mousePosition;
    QVector<QPoint> mouseTrail;
};

#endif // TOOLS_RUBBERTOOL_HPP
