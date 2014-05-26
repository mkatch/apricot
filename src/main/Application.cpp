#include "Application.hpp"
#include "Project.hpp"

/*!
  \class Application
  \inmodule main
  \brief The Application class.
 */


/*!
  Constructor.
  \a argc something.
  \a argv something.
 */
Application::Application(int argc, char** argv) :
    QApplication(argc, argv),
    m_project(new Project(this))
{
    // Do nothing
}

/*!
  Project getter.
 */
Project &Application::project()
{
    return *m_project;
}
