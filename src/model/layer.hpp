#ifndef CORE_LAYER_HPP
#define CORE_LAYER_HPP

#include <QObject>
#include <QPixmap>

#include <ApricotCore>

class AnimationFrame;

class Layer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(AnimationFrame *frame READ frame)
    Q_PROPERTY(int width READ width)
    Q_PROPERTY(int height READ height)
    Q_PROPERTY(QSize size READ size)

public:
    explicit Layer(AnimationFrame *frame);
    Layer(const Layer *other, AnimationFrame *frame);

    AnimationFrame *frame();

    const Canvas *canvas() const;

    int width() const;
    int height() const;
    QSize size() const;

    Painter getPainter();

private:
    Canvas m_canvas;

private slots:
    void updateSize();
};

inline AnimationFrame *Layer::frame()
{
    return reinterpret_cast<AnimationFrame *>(parent());
}

inline const Canvas *Layer::canvas() const
{
    return &m_canvas;
}

inline int Layer::width() const
{
    return m_canvas.width();
}

inline int Layer::height() const
{
    return m_canvas.height();
}

inline QSize Layer::size() const
{
    return m_canvas.size();
}

#endif // CORE_LAYER_HPP
