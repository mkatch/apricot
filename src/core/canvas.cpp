#include "canvas.hpp"

/*!
 * \class Canvas
 * \inmodule core
 *
 * \brief A canvas to paint on.
 *
 * It is a more restricted version of QPixmap.
 */

// Properties

/*!
 * \property Canvas::pixmap
 * \brief Returns underlying QPixmap object.
 */
/*!
 * \property Canvas::width
 * \brief Returns width of the canvas.
 */
/*!
 * \property Canvas::height
 * \brief Returns height of the canvas.
 */
/*!
 * \property Canvas::size
 * \brief Returns size of the canvas.
 */

/*!
 * \brief Creates a null canvas.
 */
Canvas::Canvas()
{
    // Do nothing.
}

/*!
 * \brief Constructs a canvas with the given \a width and \a height. If either \a width
 * or \a height is zero, a null canvas is constructed.
 */
Canvas::Canvas(int width, int height) :
    m_pixmap(width, height)
{
    // Do nothing.
}

/*!
 * \brief Constructs a canvas of the given \a size.
 */
Canvas::Canvas(const QSize &size) :
    m_pixmap(size)
{
    // Do nothing.
}

/*!
 * \brief Constructs a canvas that is a copy of given \a canvas.
 */
Canvas::Canvas(const Canvas &canvas) :
    m_pixmap(canvas.m_pixmap)
{
    // Do nothing.
}

/*!
 * \brief Constructs a canvas that has the same data as given \a pixmap.
 */
Canvas::Canvas(const QPixmap &pixmap) :
    m_pixmap(pixmap)
{
    // Do nothing.
}

/*!
 * \brief Fills the pixmap with the given color.
 */
void Canvas::fill(const QColor &color)
{
    m_pixmap.fill(color);
}

/*!
 * \brief Changes size of the canvas to given \a width and \a height.
 *
 * This is an overloaded function.
 */
void Canvas::setSize(int width, int height)
{
    setSize(QSize(width, height));
}

/*!
 * \brief Changes dimmensions of the canvas to given \a size.
 *
 * If a new dimmensions are greater than old ones empty space is filled with
 * transparency. Otherwise image is simply trimmed.
 *
 * Be aware that this method is slow and involves creating completely new QPixmap of
 * given size.
 */
void Canvas::setSize(const QSize &size)
{
    // No need to create whole new pixmap if the size doesn't actually change.
    if(size == this->size())
        return;

    // Simply create larger canvas and paint current pixmap on it. Slow but works.
    QPixmap resized(size);
    resized.fill(Qt::transparent);

    QPainter painter(&resized);
    painter.drawPixmap(0, 0, m_pixmap);

    m_pixmap.swap(resized);
}
