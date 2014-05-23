#ifndef WIDGETS_CANVASVIEW_HPP
#define WIDGETS_CANVASVIEW_HPP

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

namespace Ui {
class CanvasView;
}

class Canvas;

class CanvasFrame : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QPointF position READ position NOTIFY positionChanged)
    Q_PROPERTY(qreal x READ x NOTIFY xChanged)
    Q_PROPERTY(qreal y READ y NOTIFY yChanged)
    Q_PROPERTY(QSizeF size READ size NOTIFY sizeChanged)
    Q_PROPERTY(qreal width READ width NOTIFY widthChanged)
    Q_PROPERTY(qreal height READ height NOTIFY heightChanged)

public:
    QPointF position() const { return pixmapItem->pos(); }
    qreal x() const { return pixmapItem->x(); }
    qreal y() const { return pixmapItem->y(); }

    QSizeF size() const { return QSizeF(width(), height()); }
    qreal width() const { return pixmapItem->pixmap().width() * pixmapItem->scale(); }
    qreal height() const {return pixmapItem->pixmap().height() * pixmapItem->scale(); }

signals:
    void positionChanged();
    void xChanged();
    void yChanged();
    void sizeChanged();
    void widthChanged();
    void heightChanged();

private:
    QGraphicsPixmapItem *pixmapItem;

    CanvasFrame(QGraphicsPixmapItem *pixmapItem);

    friend class CanvasView;
};

class CanvasView : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(Canvas *canvas READ canvas WRITE setCanvas NOTIFY canvasChanged)
    Q_PROPERTY(CanvasFrame *canvasFrame READ canvasFrame)
    Q_PROPERTY(qreal zoom READ zoom WRITE setZoom NOTIFY zoomChanged)

public:
    explicit CanvasView(QWidget *parent = 0);
    ~CanvasView();

    Canvas *canvas() const { return m_canvas; }
    void setCanvas(Canvas *canvas);

    const CanvasFrame *canvasFrame() const { return &m_canvasFrame; }
    CanvasFrame *canvasFrame() { return &m_canvasFrame; }

    qreal zoom() const { return pixmapItem->scale(); }
    void setZoom(qreal zoom);

    void dragTo(const QPointF &pos);
    void dragTo(qreal x, qreal y) { dragTo(QPointF(x, y)); }

    void dragBy(const QPointF &dpos) { dragTo(canvasFrame()->position() + dpos); }
    void dragBy(qreal dx, qreal dy) { dragBy(QPointF(dx, dy)); }

signals:
    void canvasChanged();
    void zoomChanged();

private:
    Ui::CanvasView *ui;
    QPixmap placeholderPixmap;
    QGraphicsScene scene;
    QGraphicsPixmapItem *pixmapItem;

    Canvas *m_canvas;
    CanvasFrame m_canvasFrame;

    friend class CanvasFrame;
};

#endif // WIDGETS_CANVASVIEW_HPP
