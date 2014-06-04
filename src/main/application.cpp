#include "application.hpp"

#include "core/project.hpp"

/*!
  \class Application
  \inmodule main

  \brief Hold the application state and contains the entry point. It is a singleton.
 */

// Properties

/*!
    \property Application::project
    \brief The root of application data model.
 */

/*!
  \brief Constructor which is an entry point for the application. Designed to receive \a argc and
  \a argv from the call for \c main.
 */
Application::Application(int argc, char** argv) :
    QApplication(argc, argv),
    m_project(new Project(this))
{
    // Do nothing
}

Project *Application::project()
{
    return m_project;
}
