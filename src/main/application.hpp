#ifndef MAIN_APPLICATION_HPP
#define MAIN_APPLICATION_HPP

#include <QApplication>

#include <ApricotCore>

#include "mainwindow.hpp"

class Project;

class Application : public QApplication
{
    Q_OBJECT

public:
    Application(int &argc, char **argv);

    MainWindow *mainWindow();

signals:
    void projectChanged();

private:
    MainWindow m_mainWindow;
};

inline MainWindow *Application::mainWindow()
{
    return &m_mainWindow;
}

#endif // MAIN_APPLICATION_HPP
