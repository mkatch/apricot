#ifndef VIEW_ANIMATIONFRAMEVIEW_HPP
#define VIEW_ANIMATIONFRAMEVIEW_HPP

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QPixmap>

#include <ApricotModel>

class Tool;

class AnimationFrameView : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(AnimationFrame *frame READ frame WRITE setFrame NOTIFY frameChanged)
    Q_PROPERTY(Layer *activeLayer READ activeLayer WRITE setActiveLayer NOTIFY activeLayerChanged)
    Q_PROPERTY(Tool *tool READ tool WRITE setTool NOTIFY toolChanged)
    Q_PROPERTY(qreal scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(QPointF translation READ translation WRITE setTranslation NOTIFY translationChanged)
    Q_PROPERTY(QTransform transform READ transform NOTIFY transformChanged)

public:
    explicit AnimationFrameView(QWidget *parent = nullptr);
    virtual ~AnimationFrameView() = default;

    AnimationFrame *frame() const;
    Layer *activeLayer() const;
    Tool *tool();
    const Tool *tool() const;

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

    int onionSkinPrevious;
    int onionSkinNext;

public slots:
    void setFrame(AnimationFrame *frame);
    void setActiveLayer(Layer *layer);
    void setTool(Tool *tool);
    void setOnionSkinFrames();

signals:
    void frameChanged();
    void activeLayerChanged();
    void scaleChanged();
    void translationChanged();
    void transformChanged();
    void toolChanged();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    QGraphicsView *graphicsView;
    QGraphicsScene *scene;
    QGraphicsItem *frameItem;
    QPointF lastMousePos;
    Canvas backBuffer;
    QRect lastBackBufferChange;
    QGraphicsRectItem *background;

    AnimationFrame *m_frame;
    Layer *m_activeLayer;
    Tool *m_tool;

    QPixmap *onionSkin;

    void layOut();

    void revertBackBuffer();
    void toolPreview();
    void toolCommit();

    friend class Tool;
    friend class GraphicsAnimationFrameViewItem;

};

inline AnimationFrame *AnimationFrameView::frame() const
{
    return m_frame;
}

inline Layer *AnimationFrameView::activeLayer() const
{
    return m_activeLayer;
}

inline Tool *AnimationFrameView::tool()
{
    return m_tool;
}

inline const Tool *AnimationFrameView::tool() const
{
    return m_tool;
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

inline QPointF AnimationFrameView::mapToFrame(const QPointF &point) const
{
    return (point - translation()) / scale();
}

inline QPointF AnimationFrameView::mapFromFrame(const QPointF &point) const
{
    return point * scale() + translation();
}

#endif // VIEW_ANIMATIONFRAMEVIEW_HPP
