#ifndef CORE_LAYER_HPP
#define CORE_LAYER_HPP

#include <QObject>
#include <QPixmap>

#include <memory>
using std::unique_ptr;

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
    AnimationFrame *frame();
    const AnimationFrame *frame() const;

    int width() const;
    int height() const;
    QSize size() const;

    const Canvas &canvas() const;

    Painter *beginPainting();
    void endPainting();

signals:
    void contentsChanged(const QRect& rect = QRect());

private:
    AnimationFrame *m_frame;
    Canvas m_canvas;

    Painter *painter;

private slots:
    void updateSize();

private:
    explicit Layer(AnimationFrame *frame);
    Layer(const Layer* other, AnimationFrame *frame);

    friend class AnimationFrame;
};

inline AnimationFrame *Layer::frame()
{
    return m_frame;
}

inline const AnimationFrame *Layer::frame() const
{
    return m_frame;
}

inline const Canvas &Layer::canvas() const
{
    return m_canvas;
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
