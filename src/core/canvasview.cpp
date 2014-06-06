#include "canvasview.hpp"

#include <algorithm>
using std::max;

#include <ApricotUtils>
#include "canvas.hpp"

/*!
    \class CanvasView
    \inmodule core

    \brief A widget for displaying Canvas.

    The displayed canvas may be dragged around and zoomed in and out.
 */

// Properties

/*!
    \property CanvasView::canvas
    \brief The displayed Canvas.
 */
/*!
    \property CanvasView::scale
    \brief The zoom factor.
 */
/*!
    \property CanvasView::translation
    \brief The display offset.

    \sa translate()
 */
/*!
    \property CanvasView::transform
    \brief Tha transformation for transition from Canvas coordinate system to the view's
    coordinates.
 */

// Methods

/*!
    \brief Contructor.

    The created CanvasView becomes a child of \a parent.
*/
CanvasView::CanvasView(QWidget *parent) :
    QWidget(parent),
    graphicsView(new QGraphicsView(this)),
    graphicsScene(new QGraphicsScene(this)),
    placeholderPixmap(256, 256),
    m_canvas(nullptr)
{
    canvasItem = graphicsScene->addPixmap(placeholderPixmap);
    graphicsView->setScene(graphicsScene);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    graphicsView->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    layOut();
}

/*!
    \brief Destroys the CanvasView
 */
CanvasView::~CanvasView()
{
    // Do nothing
}

void CanvasView::setCanvas(const Canvas *canvas)
{
    if (m_canvas == canvas) {
        return;
    }

    m_canvas = canvas;
    if(m_canvas != nullptr) {
        canvasItem->setPixmap(m_canvas->pixmap());
    } else {
        canvasItem->setPixmap(placeholderPixmap);
    }

    emit canvasChanged();
}

void CanvasView::setScale(qreal scale)
{
    if (canvasItem->scale() == scale) {
        return;
    }

    canvasItem->setScale(scale);
    emit scaleChanged();
    emit transformChanged();
}

/*!
    \fn CanvasView::scale(qreal scale)
    \brief mutiply tcurrent scale by \a scale.
*/

void CanvasView::setTranslation(const QPointF &translation)
{
    if (canvasItem->pos() == translation) {
        return;
    }

    canvasItem->setPos(translation);
    emit translationChanged();
    emit transformChanged();
}

/*!
    \fn CanvasView::translate(const QPointF &translation)
    \brief Adds \a translation to current translation.
*/

/*!
    \fn CanvasView::translate(qreal x, qreal y)
    \brief Adds (\a x, \a y) to current translation.
 */

QTransform CanvasView::transform() const
{
    return QTransform().scale(scale(), scale()).translate(translation().x(), translation().y());
}

/*!
    \brief Maps \a point from view coordinate system to the coordinate system of the dispayed Canvas.

    \sa mapFromCanvas()
*/
QPointF CanvasView::mapToCanvas(const QPointF &point) const
{
    return (point - translation()) / scale();
}

/*!
    \brief Maps \a point from the coordinate system of the dislayed canvas to view coordinates.

    \sa mapToCanvas()
*/
QPointF CanvasView::mapFromCanvas(const QPointF &point) const
{
    return point * scale() + translation();
}

/*!
 * \brief Called when the widget is resized.
 *
 * The \a event contains the information about the resize event, e.g. the old size.
 */
void CanvasView::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    layOut();
}

/*!
    \brief Lays out the child widgets after resize and at creation.
 */
void CanvasView::layOut()
{
    graphicsView->resize(size());
    graphicsView->setSceneRect(2, 2, width() - 2, height() - 2);
}
