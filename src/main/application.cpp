#include "application.hpp"

/*!
 * \class Application
 * \inmodule main
 *
 * \brief Holds the application state and contains the entry point.
 *
 * It is a singleton.
 */

// Methods

/*!
 * \brief Constructs the application.
 *
 * This is the entry point for the application. Designed to receive \a argc and \a argv from the
 * call for \c main.
 */
Application::Application(int &argc, char** argv) :
    QApplication(argc, argv)
{
    mainWindow()->show();
}


/*!
 * \fn Application::mainWindow()
 * \brief Returns the main window of the application.
 */
