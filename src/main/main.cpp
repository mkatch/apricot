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
        layer->beginPainting()->drawImage(":/images/isocastle.png");
        layer->endPainting();
    }

    AnimationFrame *frame = project->frame(0);
    for (int i = 0; i < 3; ++i) {
        Layer *layer = frame->newLayer();
        layer->beginPainting()->drawImage(":/images/isocastle.png");
        layer->endPainting();
    }

    application.setProject(project);

    DragTool tool;
//    RectangleTool tool;
    //EllipseTool tool;
    //LineTool tool;
    application.mainWindow()->frameView()->setTool(&tool);

    return application.exec();
}
