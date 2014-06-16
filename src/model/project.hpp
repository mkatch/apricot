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
    Q_PROPERTY(int frameCount READ frameCount)

public:
    explicit Project(QObject *parent = nullptr);

    const QSize &size() const;
    void setSize (QSize const& size);
    void setSize (int width, int height);

    int width() const;
    int height() const;

    int frameCount() const;
    QList<AnimationFrame *> frames() const;

    AnimationFrame *frame(int i);
    const AnimationFrame *frame(int i) const;

    int indexOfFrame(const AnimationFrame *frame) const;

    AnimationFrame *newFrameBefore(int i);
    AnimationFrame *newFrameAfter(int i);
    AnimationFrame *newFrame();

    void deleteFrame(int i);

    void moveFrame(int from, int to);
    void moveFrame(const AnimationFrame *frame, int to);

signals:
    void sizeChanged();
    void framesChanged();
    void layersChanged();

private:
    QSize m_size;
    QList<AnimationFrame *> m_frames;

    AnimationFrame *newFrame(int i);
    void insertFrame(int i, AnimationFrame *frame);
};

inline const QSize &Project::size() const
{
    return m_size;
}

inline void Project::setSize(int width, int height)
{
    setSize(QSize(width, height));
}

inline int Project::width() const
{
    return size().width();
}

inline int Project::height() const
{
    return size().height();
}

inline int Project::frameCount() const
{
    return m_frames.count();
}

inline QList<AnimationFrame *> Project::frames() const
{
    return m_frames;
}

inline AnimationFrame *Project::frame(int i)
{
    return (0 <= i && i < frameCount()) ? m_frames.at(i) : nullptr;
}

inline const AnimationFrame *Project::frame(int i) const
{
    return (0 <= i && i < frameCount()) ? m_frames.at(i) : nullptr;
}

inline int Project::indexOfFrame(const AnimationFrame *frame) const
{
    return m_frames.indexOf(const_cast<AnimationFrame *>(frame));
}

inline AnimationFrame *Project::newFrame()
{
    return newFrameAfter(frameCount() - 1);
}

inline void Project::moveFrame(const AnimationFrame *frame, int to)
{
    moveFrame(indexOfFrame(frame), to);
}

#endif // CORE_PROJECT_HPP
