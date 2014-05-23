#include "Application.hpp"
#include "Project.hpp"


Application::Application(int argc, char** argv) :
    QApplication(argc, argv),
    m_project(new Project(this))
{
    // Do nothing
}
