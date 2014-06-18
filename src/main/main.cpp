#include <QDockWidget>

#include <ApricotTools>

#include "application.hpp"

int main(int argc, char *argv[])
{
    Application application(argc, argv);

    Project *project = new Project;

    project->setSize(918, 655);
    for (int i = 0; i < 2; i++) {
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

    // Serialization test.
    Project::save("test.apr", project);
    delete project;
    project = Project::load("test.apr");
//    project = Project::load(":/images/tv.png");
//    project = Project::load(":/images/cube.gif");

    application.mainWindow()->setProject(project);

    return application.exec();
}
