#include "layer.hpp"

#include <QPainter>

#include "Project.hpp"
#include "AnimationFrame.hpp"

Layer::Layer(AnimationFrame *frame) :
    QObject(frame),
    m_canvas(frame->project()->size())
{
    m_canvas.fill(Qt::white);
    connect(frame->project(), SIGNAL(sizeChanged()), this, SLOT(updateSize()));
}

Layer::Layer(const Layer *other, AnimationFrame *frame) :
    QObject(frame),
    m_canvas(other->m_canvas)
{
    // Do nothing
}

Painter Layer::startPainting()
{
    return Painter(m_canvas);
}

void Layer::updateSize()
{
    // This is obviously a temporary solution
    Canvas newCanvas(frame->project()->size());
    newCanvas.fill(Qt::white);
    QPainter painter(&newCanvas);
    painter.drawPixmap(0, 0, m_canvas);
    m_canvas.swap(newCanvas);
}
