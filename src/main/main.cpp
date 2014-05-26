#include <QDebug>

#include "Application.hpp"
#include "Project.hpp"
#include "MainWindow.hpp"
#include "ApricotCore.hpp"
#include "ApricotTools.hpp"

int main(int argc, char *argv[])
{
    Application application(argc, argv);

    MainWindow mainWindow;

    ToolActionArea actionArea;
    DragTool dragTool;
    actionArea.setTool(&dragTool);

//    actionArea.setCanvas(application.project().canvas());

    mainWindow.setCentralWidget(&actionArea);
    mainWindow.show();

    return application.exec();

}
