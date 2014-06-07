CONFIG += qt c++11

QT += gui core widgets

INCLUDEPATH += src

include(src/modules.pri)
include(resources/resources.pri)

OTHER_FILES += \
    Apricot.qdocconf \
    theme.css \
    qdoc-index.qdoc
