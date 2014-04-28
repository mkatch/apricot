#include "project.hpp"

#include <QDebug>
#include <QPainter>

Project::Project(QObject *parent) :
    QObject(parent),
    m_canvas(":/images/isocastle.png", this)
{
    // Do nothing
}
