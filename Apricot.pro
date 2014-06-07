CONFIG += qt c++11

QT += gui core widgets

INCLUDEPATH += src

include(src/core/core.pri)
include(src/main/main.pri)
include(src/tools/tools.pri)
include(src/utils/utils.pri)
include(src/view/view.pri)

RESOURCES += \
    resources.qrc \
    src.qrc

OTHER_FILES += \
    resources/images/isocastle.png \
    Apricot.qdocconf \
    theme.css \
    qdoc-index.qdoc

HEADERS += \
    apricotcore.hpp \
    apricottools.hpp \
    apricotutils.hpp \
    ApricotView

SOURCES +=
