#include "AnimationFrame.hpp"

#include "layer.hpp"

AnimationFrame::AnimationFrame(Project *project) :
    QObject(project)
{
    // Do nothing
}

AnimationFrame::AnimationFrame(const AnimationFrame *other, Project *project) :
    QObject(project)
{
    foreach (const Layer *layer, other->layers)
        this->layers.append(new Layer(layer, this));
}

Layer *AnimationFrame::newLayer(int i)
{
    layers.insert(at, new Layer(this));
    emit layersChanged();
}

void AnimationFrame::moveLayer(int from, int to)
{
    if (from != to) {
        layers.move(from, to);
        emit layersChanged();
    }
}

void AnimationFrame::removeLayer(int i)
{
    delete layers.takeAt(i);
    emit layersChanged();
}
