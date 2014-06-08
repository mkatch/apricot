#include "project.hpp"

#include <ApricotUtils>
#include "animationframe.hpp"

/*!
 * \class Project
 * \inmodule model
 *
 * \brief The root of Apricot data model.
 */

// Properties

/*!
 * \property Project::size
 * \brief The dimensions of the image.
 */

/*!
 * \property Project::width
 * \brief The width of the image.
 */

/*!
 * \property Project::height
 * \brief The height of the image.
 */

/*!
 * \property Project::frameCount
 * \brief The number of animation frames.
 */

// Signals

/*!
 * \fn Project::framesChanged()
 * \brief Emitted when animation frames are added, removed or change order.
 */

/*!
 * \fn Project::layersChanged()
 * \brief Emitted when layers are added, removed or change order.
 */

// Methods

/*!
 * \brief Constructs Project with parent object \a parent.
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

/*!
 * \fn Project::frame(int i)
 * \brief Returns AnimationFrame at index \a i.
 */

/*! \fn Project::frame(int i) const
 *  \brief Returns an immutable AnimationFrame at index \a i.
 */

/*!
 * \fn Project::frames() const
 * \brief Returns a list of frames that make up the animation.
 */

/*!
 * \fn Project::indexOfFrame(const AnimationFrame *frame) const
 * \brief Returns index of \a frame.
 */

/*!
 * \brief Creates new AnimationFrame at index \a i and returns it.
 *
 * The new frame is a copy of the frame at \a i - 1 if it is present.
 */
AnimationFrame *Project::newFrame(int i)
{
    i = clamp(i, 0, frameCount());

    AnimationFrame *f = 0 < i
        ? new AnimationFrame(frame(i - 1), this)
        : new AnimationFrame(this); // Typically this should only happen in an empty project

    m_frames.insert(i, f);
    connect(f, SIGNAL(layersChanged()), this, SIGNAL(layersChanged()));

    emit framesChanged();
    if (f->layerCount() > 0)
        emit layersChanged();

    return f;
}

/*!
 * \fn Project::newFrame()
 * \brief Creats new AnimationFrame at the end of animation and returns it.
 *
 * The new frame is a copy of the last frame if present.
 */

/*!
 * Removes AnimationFrame at index \a i.
 */
void Project::removeFrame(int i)
{
    AnimationFrame *f = m_frames.takeAt(i);
    bool anyLayers = f->layerCount() > 0;
    delete f;

    emit framesChanged();
    if (anyLayers)
        emit layersChanged();
}

/*!
 * \brief Moves the frame at index \a from to index \a to.
 *
 * This affects the indices of frames between \a from and \a to inclusive, as the other frames are
 * moved accordingly.
 */
void Project::moveFrame(int from, int to)
{
    if (from == to)
        return;

    m_frames.move(from, to);
    emit framesChanged();
}

/*!
 * \fn Project::moveFrame(const AnimationFrame *frame, int to)
 * \brief Moves \a frame to index \a to.
 *
 * This call is equivalent to moveFrame(indexOfFrame(frame), to).
 */
