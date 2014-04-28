#include "core.hpp"

#include <QtQml>

#include "canvas.hpp"
#include "canvasview.hpp"
#include "toolactionarea.hpp"
#include "tool.hpp"

#define REGISTER_CORE_TYPE(Type) qmlRegisterType<Type>("ApricotCore", 1, 0, #Type)
#define REGISTER_UNCREATABLE_CORE_TYPE(Type) \
    qmlRegisterUncreatableType<Type>("ApricotCore", 1, 0, #Type, #Type + QString(" is uncreatable"))

void registerCoreTypes()
{
    REGISTER_UNCREATABLE_CORE_TYPE(Canvas);
    REGISTER_CORE_TYPE(CanvasView);
    REGISTER_UNCREATABLE_CORE_TYPE(CanvasFrame);
    REGISTER_CORE_TYPE(ToolActionArea);
    REGISTER_UNCREATABLE_CORE_TYPE(Tool);
}
