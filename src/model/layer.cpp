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

// Methods

/*!
 * \fn Layer::canvas() const
 * \brief Returns the underlying canvas.
 *
 * The returned canvas is immutable. To be able to draw on the canvas, request a Painter through
 * createPainter().
 */

/*!
 * \fn Layer::newPainter()
 * \brief Creates new Painter bound to the underlying Canvas.
 *
 * \bold Warning: It is the caller's responsibility to free the created painter. Not doing this will result in
 * memory leak.
 */

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
    m_canvas(frame->size())
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
    m_canvas(other->m_canvas)
{
    connect(frame->project(), SIGNAL(sizeChanged()), this, SLOT(updateSize()));
}
