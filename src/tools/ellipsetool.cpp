#include "ellipsetool.hpp"

EllipseTool::EllipseTool(QObject *parent) :
    RectangleTool(parent)
{
    // Do nothing.
}

void EllipseTool::paint(Painter *painter, bool preview)
{
    Q_UNUSED(preview)

    if (!dragged)
        return;

    painter->drawEllipse(selectedRectangle());
}
