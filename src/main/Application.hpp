#ifndef MAIN_APPLICATION_HPP
#define MAIN_APPLICATION_HPP


#include <QGuiApplication>


class Project;

class Application : public QGuiApplication
{
    Q_OBJECT

public:
    Application(int argc, char **argv);

    Project &project() { return *m_project; }

signals:

public slots:

private:
    Project *m_project;

};


#endif // MAIN_APPLICATION_HPP
