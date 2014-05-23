CONFIG += qt c++11

QT += gui core widgets

INCLUDEPATH += src

include(src/main/main.pri)
include(src/core/core.pri)
include(src/tools/tools.pri)

RESOURCES += \
    resources.qrc \
    src.qrc

OTHER_FILES += \
    resources/images/isocastle.png

HEADERS += \
    ApricotCore.hpp \
    ApricotTools.hpp
