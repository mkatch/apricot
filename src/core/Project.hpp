#ifndef CORE_PROJECT_HPP
#define CORE_PROJECT_HPP

#include <QObject>
#include <QList>
#include <QSize>

class AnimationFrame;

class Project : public QObject
{
    Q_OBJECT
    Q_PROPERTY(const QSize &size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(int width READ width)
    Q_PROPERTY(int height READ height)

public:
    explicit Project(QObject *parent = nullptr);

    const QSize &size() const { return m_size; }
    void setSize (QSize const& size);
    void setSize (int width, int height) { setSize(QSize(width, height)); }

    int width() const { return size().width(); }
    int height() const { return size().height(); }

    int frameCount() const { return frames.count(); }

    AnimationFrame *frame(int i) { return frames.at(i); }

    AnimationFrame *newFrame(int i);
    AnimationFrame *newFrame() { return newFrame(frameCount()); }

    void removeFrame(int i);

signals:
    void sizeChanged();
    void framesChanged();
    void layersChanged();

private:
    QSize m_size;
    QList<AnimationFrame *> frames;

};


#endif // CORE_PROJECT_HPP
