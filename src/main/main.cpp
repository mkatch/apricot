#include <QDebug>
#include <QDockWidget>

#include "application.hpp"

int main(int argc, char *argv[])
{
    Application application(argc, argv);

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

    AnimationFrame *frame = application.project()->frame(0);
    frame->newLayer()->getPainter().drawImage(":/images/isocastle.png");
    application.mainWindow()->layerView()->setFrame(frame);

    return application.exec();
}
