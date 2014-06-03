#include "Project.hpp"

#include "ApricotUtils.hpp"
#include "AnimationFrame.hpp"

/*!
    \class Project
    \inmodule core

    \brief The root of Apricot data model.
 */

/*!
    Constructor.
    The \a parent is set as parent object in QObject hierarchy.
 */
Project::Project(QObject *parent) :
    QObject(parent)
{
    // Do nothing
}

/*!
    \property Project::size
    \brief The dimensions of the image
 */

void Project::setSize(const QSize &size)
{
    if (m_size != size) {
        m_size = size;
        emit sizeChanged();
    }
}

/*!
    \property Project::width
    \brief The width of the image
 */

/*!
    \property Project::height
    \brief The height of the image
 */

/*!
    \brief Creates new AnimationFrame at index \a i and returns it. The new frame is a copy of the
    frame at \a i - 1 if it is present.
 */
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

/*!
    \fn Project::newFrame()
    \brief Creats new AnimationFrame at the end of animation and returns it. The new frame is a copy
    of the last frame if present.
 */

/*!
    Removes AnimationFrame at index \a i.
 */
void Project::removeFrame(int i)
{
    AnimationFrame *f = frames.takeAt(i);
    bool anyLayers = f->layerCount() > 0;
    delete f;

    emit framesChanged();
    if (anyLayers)
        emit layersChanged();
}
