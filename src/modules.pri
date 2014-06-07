include(core/core.pri)
HEADERS += src/ApricotCore \
    src/ApricotModel

include(main/main.pri)

include(model/model.pri)
HEADERS += src/ApricotModel

include(tools/tools.pri)
HEADERS += src/ApricotTools

include(utils/utils.pri)
HEADERS += src/ApricotUtils

include(view/view.pri)
HEADERS += src/ApricotView

RESOURCES += src/src.qrc
