CONFIG += c++11

INCLUDEPATH += src

include(src/main/main.pri)
include(src/core/core.pri)
include(src/tools/tools.pri)

# Please do not modify the following two lines. Required for deployment.
include(src/qtquick2applicationviewer/qtquick2applicationviewer.pri)
qtcAddDeployment()

RESOURCES += \
    resources.qrc \
    src.qrc

OTHER_FILES += \
    resources/images/isocastle.png

HEADERS += \
    ApricotCore.hpp
