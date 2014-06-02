#include "Project.hpp"

#include "ApricotUtils.hpp"
#include "AnimationFrame.hpp"

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
    QObject(parent)
{
    // Do nothing
}

void Project::setSize(const QSize &size)
{
    if (m_size != size) {
        m_size = size;
        emit sizeChanged();
    }
}

AnimationFrame *Project::newFrame(int i)
{
    i = clamp(i, 0, frameCount());

    AnimationFrame *f = 0 < i
        ? new AnimationFrame(frame(i - 1), this)
        : new AnimationFrame(this); // Typically this should only happen in an empty project

    frames.insert(i, f);
    connect(f, SIGNAL(layersChanged()), this, SIGNAL(layersChanged()));

    emit framesChanged();
    if (f->layerCount() > 0)
        emit layersChanged();

    return f;
}

void Project::removeFrame(int i)
{
    AnimationFrame *f = frames.takeAt(i);
    bool anyLayers = f->layerCount() > 0;
    delete f;

    emit framesChanged();
    if (anyLayers)
        emit layersChanged();
}
