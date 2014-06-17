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
    Q_PROPERTY(int penSize READ penSize WRITE setPenSize)
    Q_PROPERTY(QColor penColor READ penColor WRITE setPenColor)
    Q_PROPERTY(int onionSkinBackward READ onionSkinBackward WRITE setOnionSkinBackward)
    Q_PROPERTY(int onionSkinForward READ onionSkinForward WRITE setOnionSkinForward)

public:
    explicit AnimationFrameView(QWidget *parent = nullptr);
    virtual ~AnimationFrameView() = default;

    AnimationFrame *frame() const;
    Layer *activeLayer() const;
    Tool *tool();
    const Tool *tool() const;

    int penSize() const;
    const QColor &penColor() const;
    QPen pen() const;

    qreal scale() const;
    void setScale(qreal scale);
    void scale(qreal scale);

    QPointF translation() const;
    void setTranslation(const QPointF &translation);
    void setTranslation(qreal x, qreal y);
    void translate(const QPointF &translation);
    void translate(qreal x, qreal y);

    int onionSkinBackward() const;
    int onionSkinForward() const;

    QTransform transform() const;

    QPointF mapToFrame(const QPointF &point) const;
    QPointF mapFromFrame(const QPointF &point) const;

public slots:
    void setFrame(AnimationFrame *frame);
    void setActiveLayer(Layer *layer);
    void setTool(Tool *tool);
    void setOnionSkinFrames();

    void setPenSize(int size);
    void setPenColor(const QColor &color);

    void setOnionSkinBackward(int number);
    void setOnionSkinForward(int number);

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

    int m_penSize;
    QColor m_penColor;

    int m_onionSkinBackward;
    int m_onionSkinForward;
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

inline int AnimationFrameView::onionSkinBackward() const
{
    return m_onionSkinBackward;
}

inline int AnimationFrameView::onionSkinForward() const
{
    return m_onionSkinForward;
}

inline QPointF AnimationFrameView::mapToFrame(const QPointF &point) const
{
    return (point - translation()) / scale();
}

inline QPointF AnimationFrameView::mapFromFrame(const QPointF &point) const
{
    return point * scale() + translation();
}

inline int AnimationFrameView::penSize() const
{
    return m_penSize;
}

inline const QColor &AnimationFrameView::penColor() const
{
    return m_penColor;
}

inline void AnimationFrameView::setPenSize(int size)
{
    m_penSize = size;
}

inline void AnimationFrameView::setPenColor(const QColor &color)
{
    m_penColor = color;
}

inline QPen AnimationFrameView::pen() const
{
    QPen pen;
    pen.setColor(penColor());
    pen.setWidth(penSize());
    return pen;
}

#endif // VIEW_ANIMATIONFRAMEVIEW_HPP
