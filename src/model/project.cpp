#include <ApricotUtils>
#include <ApricotModel>

#include <QFileInfo>
#include <QFile>
#include <QDataStream>
#include <QPixmap>
#include <QMovie>

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

void Project::save(QString filepath, Project *project)
{
    QFile file(filepath);
    file.open(QIODevice::WriteOnly);
    QDataStream stream(&file);

    stream << project->size();
    stream << project->frameCount();
    for (int f = project->frameCount() - 1; f >= 0; f--) {
        AnimationFrame *frame = project->frames()[f];

        stream << frame->layerCount();
        for (int l = frame->layerCount() - 1; l >= 0; l--) {
            Layer *layer = frame->layers()[l];

            stream << layer->canvas().pixmap();
        }
    }

    project->setObjectName(filepath);
}

Project *Project::load(QString filepath)
{
    QFileInfo info(filepath);

    if (info.suffix() == "apr")
        return loadApricotFile(filepath);
    if (info.suffix() == "gif")
        return loadGifFile(filepath);
    return loadImageFile(filepath);
}

Project *Project::loadApricotFile(QString filepath)
{
    QFile file(filepath);
    file.open(QIODevice::ReadOnly);
    QDataStream stream(&file);

    Project *project = new Project;

    QSize size;
    stream >> size;
    project->setSize(size);

    int frameCount;
    stream >> frameCount;
    for (int f = 0; f < frameCount; f++) {
        AnimationFrame *frame = project->newFrame();

        int layerCount;
        stream >> layerCount;
        for (int l = 0; l < layerCount; l++) {
            Layer *layer = frame->newLayer();

            QPixmap pixmap;
            stream >> pixmap;

            Painter *painter = layer->beginPainting();
            painter->drawImage(pixmap);
            layer->endPainting();
        }
    }

    project->setObjectName(filepath);
    return project;
}

Project *Project::loadGifFile(QString filepath)
{
    QMovie gif(filepath);

    Project *project = new Project;
    gif.jumpToNextFrame();
    project->setSize(gif.currentPixmap().size());

    for (int f = 0; f < gif.frameCount(); f++) {
        AnimationFrame *frame = project->newFrame();
        Layer *layer = frame->newLayer();

        Painter *painter = layer->beginPainting();
        painter->drawImage(gif.currentPixmap());
        layer->endPainting();

        gif.jumpToNextFrame();
    }

    return project;
}

Project *Project::loadImageFile(QString filepath)
{
    QPixmap image(filepath);

    Project *project = new Project;
    project->setSize(image.size());

    AnimationFrame *frame = project->newFrame();
    Layer *layer = frame->newLayer();

    Painter *painter = layer->beginPainting();
    painter->drawImage(image);
    layer->endPainting();

    return project;
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

AnimationFrame *Project::newFrameBefore(int i)
{
    AnimationFrame *frame = newFrame(i);
    insertFrame(i, frame);
    return frame;
}

AnimationFrame *Project::newFrameAfter(int i)
{
    AnimationFrame *frame = newFrame(i);
    insertFrame(i + 1, frame);
    return frame;
}

/*!
 * \brief Creats new AnimationFrame at the end of animation and returns it.
 */
AnimationFrame *Project::newFrame()
{
    AnimationFrame *frame = new AnimationFrame(this);
    insertFrame(frameCount(), frame);
    return frame;
}

/*!
 * Removes AnimationFrame at index \a i.
 */
void Project::deleteFrame(int i)
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

AnimationFrame *Project::newFrame(int i)
{
    return (0 <= i && i <= frameCount())
        ? new AnimationFrame(frame(i), this)
        : new AnimationFrame(this);
}

void Project::insertFrame(int i, AnimationFrame *frame)
{
    m_frames.insert(i, frame);
    connect(frame, SIGNAL(layersChanged()), this, SIGNAL(layersChanged()));

    emit framesChanged();
    if (frame->layerCount() > 0)
        emit layersChanged();
}
