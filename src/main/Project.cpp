#include "Project.hpp"

#include <QDebug>
#include <QPainter>

/*!
  \class Project
  \inmodule main
  \brief The Main Window class.
 */

/*!
  Constructor.
  \a parent object.
 */
Project::Project(QObject *parent) :
    QObject(parent),
    m_canvas(":/images/isocastle.png", this)
{
    // Do nothing
}

/*!
  Canvas getter.
 */
Canvas *Project::canvas()
{
    return &m_canvas;
}
