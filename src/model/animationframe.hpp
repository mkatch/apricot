#ifndef CORE_ANIMATIONFRAME_HPP
#define CORE_ANIMATIONFRAME_HPP

#include <QObject>
#include <QList>
#include <QSize>

class Project;
class Layer;

class AnimationFrame : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Project *project READ project)
    Q_PROPERTY(const QSize &size READ size)
    Q_PROPERTY(int width READ width)
    Q_PROPERTY(int height READ height)
    Q_PROPERTY(int layerCount READ layerCount)

public:
    Project *project();
    const Project *project() const;

    const QSize &size() const;
    int width() const;
    int height() const;

    int layerCount() const;
    QList<Layer *> layers() const;

    Layer *layer(int i);
    const Layer *layer(int i) const;

    int indexOfLayer(const Layer *layer);

    Layer *newLayer(int i);
    Layer *newLayer();

    void deleteLayer(int i);

    void moveLayer(int from, int to);
    void moveLayer(const Layer *layer, int to);

signals:
    void layersChanged();

private:
    Project *m_project;
    QList<Layer *> m_layers;

    explicit AnimationFrame(Project *project);
    AnimationFrame(const AnimationFrame *other, Project *project);

    friend class Project;
};

inline Project *AnimationFrame::project()
{
    return m_project;
}

inline const Project *AnimationFrame::project() const
{
    return m_project;
}

inline int AnimationFrame::width() const
{
    return size().width();
}

inline int AnimationFrame::height() const
{
    return size().height();
}

inline int AnimationFrame::layerCount() const
{
    return m_layers.count();
}

inline QList<Layer *> AnimationFrame::layers() const
{
    return m_layers;
}

inline Layer *AnimationFrame::layer(int i)
{
    return m_layers.at(i);
}

inline const Layer *AnimationFrame::layer(int i) const
{
    return m_layers.at(i);
}

inline int AnimationFrame::indexOfLayer(const Layer *layer)
{
    return m_layers.indexOf(const_cast<Layer *>(layer));
}

inline Layer *AnimationFrame::newLayer()
{
    return newLayer(0);
}

inline void AnimationFrame::moveLayer(const Layer *layer, int to)
{
    moveLayer(indexOfLayer(layer), to);
}

#endif // CORE_ANIMATIONFRAME_HPP
