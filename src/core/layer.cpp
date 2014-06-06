#include "layer.hpp"

#include <QPainter>

#include "canvas.hpp"
#include "project.hpp"
#include "animationframe.hpp"

/*!
    \class Layer
    \inmodule core

    \brief An image layer.

    Provides a Canvas to draw on via QPainter. Every AnimationFram may contain multiple layers.
 */

// Properties

/*!
    \property Layer::frame
    \brief The parent frame.

    It is also the QObject parent of the layer.
 */
/*!
    \property Layer::width
    \brief The width of the canvas
 */
/*!
    \property Layer::height
    \brief The height of the canvas
 */
/*!
    \property Layer::size
    \brief The dimenstions of the canvas
 */

// Getters

/*!
    \fn const Canvas *Layer::canvas() const
    \brief Returns immutable pointer to underlying Canvas object.
 */
/*!
    \fn Canvas *Layer::canvas()
    \brief Returns pointer to underlying Canvas objects.

    \preliminary
    This function is only temporary solution and will be removed in the near future.
 */

// Methods

/*!
    \brief Constructor.

    The \a frame is set as QObject parent of the layer.
 */
Layer::Layer(AnimationFrame *frame) :
    QObject(frame),
    m_canvas(frame->project()->size())
{
    m_canvas.fill(Qt::white);
    connect(frame->project(), SIGNAL(sizeChanged()), this, SLOT(updateSize()));
}

/*!
    \brief Contructor used for duplicating layers.

    The newly created layer gets a copy of the underlying Canvas of \a other. The \a frame is set as
    the QObject parent of the layer.
 */
Layer::Layer(const Layer *other, AnimationFrame *frame) :
    QObject(frame),
    m_canvas(other->m_canvas)
{
    updateSize();
}

/*!
    \brief Called when the dimensions of the project change.

    Updates the dimensions of the undelying canvas to be in sync with the rest.
 */
void Layer::updateSize()
{
    m_canvas.setSize(frame()->project()->size());
}
