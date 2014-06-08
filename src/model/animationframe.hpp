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
    explicit AnimationFrame(Project *project);
    AnimationFrame(const AnimationFrame *other, Project *project);

    Project *project();
    const Project *project() const;

    const QSize &size() const;
    int width() const;
    int height() const;

    int layerCount() const;
    QList<Layer *> layers() const;

    Layer *layer(int i);
    const Layer *layer(int i) const;

    Layer *newLayer(int i);
    Layer *newLayer();

    void moveLayer(int from, int to);

    void removeLayer(int i);

signals:
    void layersChanged();

private:
    QList<Layer *> m_layers;
};

inline Project *AnimationFrame::project()
{
    return reinterpret_cast<Project *>(parent());
}

inline const Project *AnimationFrame::project() const
{
    return reinterpret_cast<Project *>(parent());
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

inline Layer *AnimationFrame::newLayer()
{
    return newLayer(0);
}

#endif // CORE_ANIMATIONFRAME_HPP
