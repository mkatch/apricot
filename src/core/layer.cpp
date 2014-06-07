#include "layer.hpp"

#include <QPainter>

#include "project.hpp"
#include "animationframe.hpp"

/*!
 * \class Layer
 * \inmodule core
 *
 * \brief An image layer.
 *
 * Provides a Canvas to draw on via Painter. Every AnimationFram may contain multiple layers.
 */

// Properties

/*!
 * \property Layer::frame
 * \brief The parent frame.
 *
 *  It is also the QObject parent of the layer.
 */

/*!
 * \property Layer::canvas
 * \brief The underlying Canvas.
 *
 * The returned canvas is immutable. To be able to draw on the canvas, request a Painter through
 * startPainting().
 *
 * \sa startPainting()
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
 * \brief Constructor.
 *
 * The \a frame is set as QObject parent of the layer.
 */
Layer::Layer(AnimationFrame *frame) :
    QObject(frame),
    m_canvas(frame->size())
{
    m_canvas.fill(Qt::white);
    connect(frame->project(), SIGNAL(sizeChanged()), this, SLOT(updateSize()));
}

/*!
 * \brief Contructor used for duplicating layers.
 *
 * The newly created layer gets a copy of the underlying Canvas of \a other. The \a frame is set as
 * the QObject parent of the layer.
 */
Layer::Layer(const Layer *other, AnimationFrame *frame) :
    QObject(frame),
    m_canvas(other->m_canvas)
{
    updateSize();
}

/*!
 * \brief Gets a Painter bound to the undelying Canvas.
 */
Painter Layer::getPainter()
{
    return Painter(m_canvas);
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
