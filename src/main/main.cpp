#include <QDebug>

#include "Application.hpp"
#include "Project.hpp"
#include "MainWIndow.hpp"
#include "ApricotCore.hpp"
#include "ApricotTools.hpp"


int main(int argc, char *argv[])
{
    Application application(argc, argv);

    MainWindow mainWindow;
    ToolActionArea actionArea;
    DragTool dragTool;
    mainWindow.setCentralWidget(&actionArea);
    //actionArea.setCanvas(application.project().canvas());
    actionArea.setTool(&dragTool);

    mainWindow.show();

    return application.exec();
}
