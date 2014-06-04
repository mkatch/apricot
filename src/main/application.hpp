#ifndef MAIN_APPLICATION_HPP
#define MAIN_APPLICATION_HPP

#include <QApplication>

class Project;

class Application : public QApplication
{
    Q_OBJECT
    Q_PROPERTY(Project *project READ project)

public:
    Application(int &argc, char **argv);

    Project *project();

private:
    Project *m_project;

};


#endif // MAIN_APPLICATION_HPP
