#include <QQmlEngine>
#include <QQmlContext>
#include <QDebug>

#include "qtquick2applicationviewer.h"
#include "Application.hpp"
#include "Project.hpp"
#include "core/core.hpp"
#include "tools/tools.hpp"

int main(int argc, char *argv[])
{
    Application application(argc, argv);

    registerCoreTypes();
    registerTools();

    QtQuick2ApplicationViewer viewer;
    viewer.rootContext()->setContextProperty("project", &application.project());
    viewer.setSource(QUrl("qrc:/main/main.qml"));
    viewer.show();

    return application.exec();
}
