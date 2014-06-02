#ifndef WIDGETS_CANVASVIEW_HPP
#define WIDGETS_CANVASVIEW_HPP

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPixmap>

#include "Canvas.hpp" // TODO: remove

class CanvasView : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(const Canvas *canvas READ canvas WRITE setCanvas NOTIFY canvasChanged)
    Q_PROPERTY(qreal scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(QPointF translation READ translation WRITE setTranslation NOTIFY translationChanged)
    Q_PROPERTY(QTransform transform READ transform NOTIFY transformChanged)

public:
    explicit CanvasView(QWidget *parent = 0);
    virtual ~CanvasView();

    const Canvas *canvas() const { return m_canvas; }
    void setCanvas(const Canvas *canvas);

    qreal scale() const { return canvasItem->scale(); }
    void setScale(qreal scale);
    void scale(qreal scale) { setScale(this->scale() * scale); }

    QPointF translation() const { return canvasItem->pos(); }
    void setTranslation(const QPointF &translation);
    void setTranslation(qreal x, qreal y) { setTranslation(QPointF(x, y)); }
    void translate(const QPointF &translation) { setTranslation(this->translation() + translation);}
    void translate(qreal x, qreal y) { translate(QPointF(x, y)); }

    QTransform transform() const;

    QPointF mapToCanvas(const QPointF &point) const;
    QPointF mapFromCanvas(const QPointF &point) const;

signals:
    void canvasChanged();
    void scaleChanged();
    void translationChanged();
    void transformChanged();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QGraphicsView *graphicsView;
    QGraphicsScene *graphicsScene;
    QGraphicsPixmapItem *canvasItem;
    QPixmap placeholderPixmap;

    const Canvas *m_canvas;

    void layOut();
};

#endif // WIDGETS_CANVASVIEW_HPP
