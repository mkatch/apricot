#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <QGuiApplication>

class Project;

class Application : public QGuiApplication
{
    Q_OBJECT

public:
    Application(int argc, char **argv);

    Project& project();

signals:

public slots:

private:
    Project* m_project;

};

inline Project& Application::project()
{
    return *m_project;
}

#endif // APPLICATION_HPP
