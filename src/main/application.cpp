#include "application.hpp"

/*!
 * \class Application
 * \inmodule main
 *
 * \brief Holds the application state and contains the entry point.
 *
 * It is a singleton.
 */

// Properties

/*!
 * \property Application::project
 * \brief The root of application data model.
 *
 * The application takes ownership of the project set as the value of this property.
 */

// Methods

/*!
 * \brief Constructs the application.
 *
 * This is the entry point for the application. Designed to receive \a argc and \a argv from the
 * call for \c main.
 */
Application::Application(int &argc, char** argv) :
    QApplication(argc, argv),
    m_project(new Project(this))
{
    mainWindow()->animationView()->setProject(project());
    mainWindow()->show();
}

void Application::setProject(Project *project)
{
    if (m_project == project)
        return;

    m_project = project;
    mainWindow()->setProject(project);
    emit projectChanged();
}


/*!
 * \fn Application::mainWindow()
 * \brief Returns the main window of the application.
 */
