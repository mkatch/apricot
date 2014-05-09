#include "core.hpp"

#include <QtQml>

#include "Canvas.hpp"
#include "CanvasView.hpp"
#include "ToolActionArea.hpp"
#include "Tool.hpp"


#define REGISTER_CORE_TYPE(Type) qmlRegisterType<Type>("Apricot", 1, 0, #Type)
#define REGISTER_UNCREATABLE_CORE_TYPE(Type) \
    qmlRegisterUncreatableType<Type>("Apricot", 1, 0, #Type, #Type + QString(" is uncreatable"))


void registerCoreTypes()
{
    REGISTER_UNCREATABLE_CORE_TYPE(Canvas);
    REGISTER_CORE_TYPE(CanvasView);
    REGISTER_UNCREATABLE_CORE_TYPE(CanvasFrame);
    REGISTER_CORE_TYPE(ToolActionArea);
    REGISTER_UNCREATABLE_CORE_TYPE(Tool);
}
