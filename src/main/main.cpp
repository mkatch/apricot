#include <QDebug>
#include <QDockWidget>

#include "application.hpp"
#include "mainwindow.hpp"
#include "apricotcore.hpp"
#include "apricottools.hpp"

#include <ApricotView>

int main(int argc, char *argv[])
{
    Application application(argc, argv);
    MainWindow mainWindow;

    application.project()->setSize(918, 655);
    //Layer *layer = application.project()->newFrame()->newLayer();
    application.project()->newFrame()->newLayer()->startPainting().load(":/images/isocastle.png");

    application.project()->newFrame()->newLayer()->startPainting().load(":/images/isocastle.png");

    application.project()->newFrame()->newLayer()->startPainting().load(":/images/isocastle.png");

    application.project()->newFrame()->newLayer()->startPainting().load(":/images/isocastle.png");

    application.project()->newFrame()->newLayer()->startPainting().load(":/images/isocastle.png");

    application.project()->newFrame()->newLayer()->startPainting().load(":/images/isocastle.png");

    application.project()->newFrame()->newLayer()->startPainting().load(":/images/isocastle.png");

    application.project()->newFrame()->newLayer()->startPainting().load(":/images/isocastle.png");

    application.project()->newFrame()->newLayer()->startPainting().load(":/images/isocastle.png");
    //Painter painter = layer->startPainting();
    //painter.load(":/images/isocastle.png");

    ToolActionArea actionArea;
    DragTool dragTool;
    actionArea.setTool(&dragTool);
    //actionArea.setCanvas(layer->canvas());


    mainWindow.setCentralWidget(&actionArea);
    QDockWidget dockWidget;
    AnimationView animationView;
    animationView.setProject(application.project());
    dockWidget.setWidget(&animationView);
    mainWindow.addDockWidget(Qt::BottomDockWidgetArea, &dockWidget);
    mainWindow.show();

    return application.exec();
}
