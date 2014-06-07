#ifndef CORE_ANIMATION_FRAME_HPP
#define CORE_ANIMATION_FRAME_HPP

#include <QObject>
#include <QList>
#include <QSize>

class Project;
class Layer;

class AnimationFrame : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Project *project READ project)
    Q_PROPERTY(QList<Layer *> layers READ layers NOTIFY layersChanged)
    Q_PROPERTY(int layerCount READ layerCount)
    Q_PROPERTY(const QSize &size READ size)
    Q_PROPERTY(int width READ width)
    Q_PROPERTY(int height READ height)

public:
    explicit AnimationFrame(Project *project);
    AnimationFrame(const AnimationFrame *other, Project *project);

    Project *project() { return reinterpret_cast<Project *>(parent()); }
    const Project *project() const { return reinterpret_cast<const Project*>(parent()); }

    int layerCount() const { return m_layers.count(); }

    const QSize &size() const;
    int width() const { return size().width(); }
    int height() const { return size().height(); }

    QList<Layer *> layers() const { return m_layers; }
    Layer *layer(int i) { return m_layers.at(i); }
    const Layer *layer(int i) const { return m_layers.at(i); }

    Layer *newLayer(int i);
    Layer *newLayer() { return newLayer(0); }

    void moveLayer(int from, int to);

    void removeLayer(int i);

signals:
    void layersChanged();

private:
    QList<Layer *> m_layers;
};

#endif // CORE_ANIMATION_FRAME_HPP
