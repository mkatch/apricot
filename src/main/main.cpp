#include <QDebug>
#include <QDockWidget>

#include <ApricotCore>
#include <ApricotTools>

#include "application.hpp"
#include "mainwindow.hpp"

#include <ApricotView>

int main(int argc, char *argv[])
{
    Application application(argc, argv);
    MainWindow mainWindow;

    application.project()->setSize(918, 655);
    Layer *layer = application.project()->newFrame()->newLayer();
    layer->getPainter().drawImage(":/images/isocastle.png");
    application.project()->newFrame()->newLayer()->getPainter().drawImage(":/images/isocastle.png");
    application.project()->newFrame()->newLayer()->getPainter().drawImage(":/images/isocastle.png");
    application.project()->newFrame()->newLayer()->getPainter().drawImage(":/images/isocastle.png");
    application.project()->newFrame()->newLayer()->getPainter().drawImage(":/images/isocastle.png");
    application.project()->newFrame()->newLayer()->getPainter().drawImage(":/images/isocastle.png");
    application.project()->newFrame()->newLayer()->getPainter().drawImage(":/images/isocastle.png");
    application.project()->newFrame()->newLayer()->getPainter().drawImage(":/images/isocastle.png");

    ToolActionArea actionArea;
    DragTool dragTool;
    actionArea.setTool(&dragTool);
    actionArea.setCanvas(layer->canvas());

    mainWindow.setCentralWidget(&actionArea);
    QDockWidget dockWidget;
    AnimationView animationView;
    animationView.setProject(application.project());
    dockWidget.setWidget(&animationView);
    mainWindow.addDockWidget(Qt::BottomDockWidgetArea, &dockWidget);
    mainWindow.show();

    return application.exec();
}
