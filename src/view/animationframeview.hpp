#ifndef VIEW_ANIMATIONFRAMEVIEW_HPP
#define VIEW_ANIMATIONFRAMEVIEW_HPP

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>

#include <ApricotModel>

class AnimationFrameView : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(const AnimationFrame *frame READ frame WRITE setFrame NOTIFY frameChanged)
    Q_PROPERTY(qreal scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(QPointF translation READ translation WRITE setTranslation NOTIFY translationChanged)
    Q_PROPERTY(QTransform transform READ transform NOTIFY transformChanged)

public:
    explicit AnimationFrameView(QWidget *parent = 0);
    virtual ~AnimationFrameView() = default;

    const AnimationFrame *frame() const;
    void setFrame(const AnimationFrame *frame);

    qreal scale() const;
    void setScale(qreal scale);
    void scale(qreal scale);

    QPointF translation() const;
    void setTranslation(const QPointF &translation);
    void setTranslation(qreal x, qreal y);
    void translate(const QPointF &translation);
    void translate(qreal x, qreal y);

    QTransform transform() const;

    QPointF mapToFrame(const QPointF &point) const;
    QPointF mapFromFrame(const QPointF &point) const;

signals:
    void frameChanged();
    void scaleChanged();
    void translationChanged();
    void transformChanged();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QGraphicsView *graphicsView;
    QGraphicsScene *scene;
    QGraphicsItem *frameItem;
    QPixmap placeholderPixmap;

    const AnimationFrame *m_frame;

    void layOut();
};

inline const AnimationFrame *AnimationFrameView::frame() const
{
    return m_frame;
}

inline qreal AnimationFrameView::scale() const
{
    return frameItem->scale();
}

inline void AnimationFrameView::scale(qreal scale)
{
    setScale(this->scale() * scale);
}

inline QPointF AnimationFrameView::translation() const
{
    return frameItem->pos();
}

inline void AnimationFrameView::setTranslation(qreal x, qreal y)
{
    setTranslation(QPointF(x, y));
}

inline void AnimationFrameView::translate(const QPointF &translation)
{
    setTranslation(this->translation() + translation);
}

inline void AnimationFrameView::translate(qreal x, qreal y)
{
    translate(QPointF(x, y));
}

#endif // VIEW_ANIMATIONFRAMEVIEW_HPP
