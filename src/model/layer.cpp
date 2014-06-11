#include "layer.hpp"

#include <QPainter>

#include "project.hpp"
#include "animationframe.hpp"

/*!
 * \class Layer
 * \inmodule model
 *
 * \brief An image layer.
 *
 * Provides a Canvas to draw on via Painter. Every AnimationFram may contain multiple layers.
 */

// Properties

/*!
 * \property Layer::frame
 * \brief The parent frame.
 */

/*!
 * \property Layer::width
 * \brief The width of the canvas
 */

/*!
 * \property Layer::height
 * \brief The height of the canvas
 */

/*!
 * \property Layer::size
 * \brief The dimenstions of the canvas
 */

// Signals

/*!
 * \fn Layer::contentsChanged(const QRect& rect)
 * \brief Emmited after the underlying Canvas was painted on.
 *
 * A bounding rectangle containig all pixels that changed is given by \a rect. This signal may
 * sometimes be emitted even if the canvas does not actually change and also the bounding rectangle
 * may not be optimal. But it is guaranteed to be emitted with a rectangle enclosing all change
 * when they occur.
 *
 * \sa beginPainting(), endPainting()
 */

// Methods

/*!
 * \fn Layer::canvas() const
 * \brief Returns the underlying canvas.
 *
 * The returned canvas is immutable. To be able to draw on the canvas, request a Painter through
 * createPainter().
 */

/*!
 * \brief Initiates painting.
 *
 * Returns a new Painter bound to the underlying Canvas. Every call to beginPaint() must be matched
 * by a following call to endPainting(). The returned Painter is owned by Layer and is freed in
 * endPainting(), therefore not valid after the ending call.
 */
Painter *Layer::beginPainting()
{
    if (painter != nullptr) {
        qWarning(
            "Layer::beginPainting(): Previous call to beginPainting() was not matched by "
            "a following call to endPainting()"
        );
        endPainting();
    }

    painter = new Painter(m_canvas);
    return painter;
}

/*!
 * \brief Finalizes painting.
 *
 * Frees the Painter created in tha matching preceding call to beginPainting(). The painter is
 * therefore no more valid and referencing it may lead to memory corruption. Before the method
 * returns contentsChanged() signal is emitted to notify about the contents changed during
 * painting.
 */
void Layer::endPainting()
{
    if (painter == nullptr) {
        qWarning(
            "Layer::endPainting(): Calling endPainting() without prior "
            "call to beginPainting()"
        );
        return;
    }

    emit contentsChanged(painter->boundingBox());
    delete painter;
    painter = nullptr;
}

/*!
 * \brief Called when the dimensions of the project change.
 *
 * Updates the dimensions of the undelying canvas to be in sync with the rest.
 */
void Layer::updateSize()
{
    m_canvas.setSize(frame()->project()->size());
}

/*!
 * \brief Constructs a new layer with parent frame \a frame.
 *
 * The parent frame also becomes the parent object of the layer.
 */
Layer::Layer(AnimationFrame *frame) :
    QObject(frame),
    m_frame(frame),
    m_canvas(frame->size()),
    painter(nullptr)
{
    m_canvas.fill(Qt::transparent);
    connect(frame->project(), SIGNAL(sizeChanged()), this, SLOT(updateSize()));
}

/*!
 * \brief Consttructs a layer with the underlying canvas copied from \a other and parent frame
 * \a frame.
 *
 * The parent frame also becomes the parent object of the layer. This constructor can only be called
 * by AnimationFrame and is guaranteed to be provided \a frame with the same dimensions as
 * \a other.
 */
Layer::Layer(const Layer *other, AnimationFrame *frame) :
    QObject(frame),
    m_frame(frame),
    m_canvas(other->m_canvas),
    painter(nullptr)
{
    connect(frame->project(), SIGNAL(sizeChanged()), this, SLOT(updateSize()));
}
