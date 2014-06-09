#include <QDebug>
#include <QDockWidget>

#include <ApricotTools>

#include "application.hpp"

int main(int argc, char *argv[])
{
    Application application(argc, argv);

    Project *project = new Project;

    project->setSize(918, 655);
    Layer *layer = project->newFrame()->newLayer();
    layer->getPainter().drawImage(":/images/isocastle.png");
    project->newFrame()->newLayer()->getPainter().drawImage(":/images/isocastle.png");
    project->newFrame()->newLayer()->getPainter().drawImage(":/images/isocastle.png");
    project->newFrame()->newLayer()->getPainter().drawImage(":/images/isocastle.png");
    project->newFrame()->newLayer()->getPainter().drawImage(":/images/isocastle.png");
    project->newFrame()->newLayer()->getPainter().drawImage(":/images/isocastle.png");
    project->newFrame()->newLayer()->getPainter().drawImage(":/images/isocastle.png");
    project->newFrame()->newLayer()->getPainter().drawImage(":/images/isocastle.png");

    AnimationFrame *frame = project->frame(0);
    frame->newLayer()->getPainter().drawImage(":/images/isocastle.png");

    application.setProject(project);

    DragTool dragTool;
    application.mainWindow()->actionArea()->setTool(&dragTool);

    return application.exec();
}
