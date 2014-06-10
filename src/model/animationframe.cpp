#include "animationframe.hpp"

#include "layer.hpp"
#include "project.hpp"

/*!
 * \class AnimationFrame
 * \inmodule model
 *
 * \brief A single frame of animation.
 *
 * May contain multiple layers.
 */

// Properties

/*!
 * \property AnimationFrame::project
 * \brief The parent Project.
 */

/*!
 * \property AnimationFrame::size
 * \brief The dimensins of the frame
 */

/*!
 * \property AnimationFrame::width
 * \brief The width of the frame.
 */

/*!
 * \property AnimationFrame::height
 * \brief The height of the frame.
 */

/*!
 * \property AnimationFrame::layerCount
 * \brief The number of layers
 */

// Signals

/*!
 * \fn AnimationFrame::layersChanged()
 * \brief Emitted when layers are added, removed or change order.
 */

// Methods

const QSize &AnimationFrame::size() const
{
    // This has to be implemented here because of dependency loop that would arise if
    // animationframe.hpp included project.hpp.
    return project()->size();
}

/*!
 * \fn AnimationFrame::layers()
 * \brief Returnst the list of layers that make up the animation frame.
 */

/*!
 * \brief Returns a list of immutable layers that make up the animation frame.
 */
QList<const Layer *> AnimationFrame::layers() const
{
    QList<const Layer *> result;
    result.reserve(m_layers.count());
    foreach (Layer *layer, m_layers)
        result.append(layer);
    return result;
}

/*!
 * \fn AnimationFrame::layer(int i)
 * \brief Returns layer at index \a i.
 *
 * The topmost layer has index 0.
 */

/*!
 * \fn AnimationFrame::layer(int i) const
 * \brief Returns immutable layer at index \a i.
 *
 * The topmost layer has index 0.
 */

/*!
 * \fn AnimationFrame::indexOfLayer(const Layer *layer) const
 * \brief Returns the index of \a layer.
 *
 * The topmost layer has index 0. If \a layer is not member of this AnimationFrame, -1 is returned.
 */

/*!
 * \brief Creates a new layer at index \a i and returns it.
 */
Layer *AnimationFrame::newLayer(int i)
{
    Layer *l = new Layer(this);
    m_layers.insert(i, l);
    emit layersChanged();
    return l;
}

/*!
 * \fn AnimationFrame::newLayer()
 * \brief Creates a new layer above all existing ones and returns it.
 */

/*!
 * \brief Moves layer from position \a from to position \a to.
 *
 * This affects the indices of layers between \a from and \a to inclusive, as the other layers are
 * moved accordingly.
 *
 * \overload
 */
void AnimationFrame::moveLayer(int from, int to)
{
    if (from != to) {
        m_layers.move(from, to);
        emit layersChanged();
    }
}

/*!
 * \fn AnimationFrame::moveLayer(const Layer *layer, int to)
 * \brief Moves \a layer to position \a to.
 *
 * For this method to work, \a layer must be a member of this AnimationFrame. This is equivalent to
 * moveLayer(indexOfLayer(layer), to).
 *
 * \overload
 */

/*!
 * \brief Deletes layer at index \a i.
 *
 * The destructor of the layer is called and it's memory is freed.
 */
void AnimationFrame::deleteLayer(int i)
{
    delete m_layers.takeAt(i);
    emit layersChanged();
}

/*!
 * \brief Constructs a frame with parent project \a project.
 *
 * The \a project also becomes the parent object of the newly created AnimationFrame.
 */
AnimationFrame::AnimationFrame(Project *project) :
    QObject(project),
    m_project(project)
{
    // Do nothing
}

/*!
 * Constructs a frame with all layers copied from \a other and with parent project \a project.
 *
 * The parent project also becomes the parent object of the frame. This constructor can only be
 * called by Project and is guaranteed to be provided \a project with the same dimensions as
 * \a other.
*/
AnimationFrame::AnimationFrame(const AnimationFrame *other, Project *project) :
    QObject(project),
    m_project(project)
{
    foreach (const Layer *layer, other->m_layers)
        this->m_layers.append(new Layer(layer, this));
}
