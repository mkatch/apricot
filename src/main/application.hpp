#ifndef MAIN_APPLICATION_HPP
#define MAIN_APPLICATION_HPP

#include <QApplication>

#include <ApricotCore>

#include "mainwindow.hpp"

class Project;

class Application : public QApplication
{
    Q_OBJECT
    Q_PROPERTY(Project *project READ project WRITE setProject NOTIFY projectChanged)

public:
    Application(int &argc, char **argv);

    MainWindow *mainWindow();

    Project *project();
    void setProject(Project *project);

signals:
    void projectChanged();

private:
    MainWindow m_mainWindow;

    Project *m_project;
};

inline MainWindow *Application::mainWindow()
{
    return &m_mainWindow;
}

inline Project *Application::project()
{
    return m_project;
}

#endif // MAIN_APPLICATION_HPP
