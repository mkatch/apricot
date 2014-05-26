#ifndef MAIN_APPLICATION_HPP
#define MAIN_APPLICATION_HPP


#include <QApplication>


class Project;

class Application : public QApplication
{
    Q_OBJECT

public:
    Application(int argc, char **argv);

    Project &project();

signals:

public slots:

private:
    Project *m_project;

};


#endif // MAIN_APPLICATION_HPP
