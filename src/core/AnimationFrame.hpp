#ifndef CORE_ANIMATION_FRAME_HPP
#define CORE_ANIMATION_FRAME_HPP

#include <QObject>
#include <QList>

class Project;
class Layer;

class AnimationFrame : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Project *project READ project)
    Q_PROPERTY(int layerCount READ layerCount)

public:
    explicit AnimationFrame(Project *project);
    AnimationFrame(const AnimationFrame *other, Project *project);

    Project *project() { return reinterpret_cast<Project *>(parent()); }

    int layerCount() const { return layers.count(); }

    Layer *layer(int i) { return layers.at(i); }

    Layer *newLayer(int i);
    Layer *newLayer() { return newLayer(0); }

    void moveLayer(int from, int to);

    void removeLayer(int i);

signals:
    void layersChanged();

private:
    QList<Layer *> layers;
};

#endif // CORE_ANIMATION_FRAME_HPP
