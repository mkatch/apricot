#include <QDebug>
#include <QDockWidget>

#include <ApricotTools>

#include "application.hpp"

int main(int argc, char *argv[])
{
    Application application(argc, argv);

    Project *project = new Project;

    project->setSize(918, 655);
    for (int i = 0; i < 8; i++) {
        Layer *layer = project->newFrame()->newLayer();
        Painter *painter = layer->newPainter();
        painter->drawImage(":/images/isocastle.png");
        delete painter;
    }

    AnimationFrame *frame = project->frame(0);
    Layer *layer = frame->newLayer();
    Painter *painter = layer->newPainter();
    painter->drawImage(":/images/isocastle.png");
    delete painter;

    application.setProject(project);

    DragTool dragTool;
    application.mainWindow()->actionArea()->setTool(&dragTool);

    return application.exec();
}
