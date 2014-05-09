#include "tools.hpp"

#include <QtQml>

#include "DragTool.hpp"


#define REGISTER_TOOL(Tool) qmlRegisterType<Tool>("Apricot.Tools", 1, 0, #Tool)


void registerTools()
{
    REGISTER_TOOL(DragTool);
}
