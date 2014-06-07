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

    return application.exec();
}
