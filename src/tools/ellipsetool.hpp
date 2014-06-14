#ifndef TOOLS_ELLIPSETOOL_HPP
#define TOOLS_ELLIPSETOOL_HPP

#include <QObject>

#include "view/tool.hpp"
#include "rectangletool.hpp"

class EllipseTool : public RectangleTool
{
    Q_OBJECT

public:
    explicit EllipseTool(QObject *parent = nullptr);

protected:
    void paint(Painter *painter, bool preview) override;
};

#endif // TOOLS_ELLIPSETOOL_HPP
