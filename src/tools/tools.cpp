#include "tools.hpp"

#include <QtQml>

#include "dragtool.hpp"

#define REGISTER_TOOL(Tool) qmlRegisterType<Tool>("ApricotTools", 1, 0, #Tool)

void registerTools()
{
    REGISTER_TOOL(DragTool);
}
