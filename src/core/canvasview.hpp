#ifndef CANVASVIEW_HPP
#define CANVASVIEW_HPP

#include <QQuickItem>

#include "canvasprovider.hpp"

class CanvasView;
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
    QPointF position() const { return imageItem->position(); }
    qreal x() const { return imageItem->x(); }
    qreal y() const { return imageItem->y(); }

    QSizeF size() const { return QSizeF(width(), height()); }
    qreal width() const { return imageItem->width() * imageItem->scale(); }
    qreal height() const { return imageItem->height() * imageItem->scale(); }

signals:
    void positionChanged();
    void xChanged();
    void yChanged();
    void sizeChanged();
    void widthChanged();
    void heightChanged();

private:
    const QQuickItem *imageItem;

    CanvasFrame(CanvasView* view);

    friend class CanvasView;
};

class CanvasView : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(Canvas *canvas READ canvas WRITE setCanvas NOTIFY canvasChanged)
    Q_PROPERTY(CanvasFrame *canvasFrame READ canvasFrame)
    Q_PROPERTY(qreal zoom READ zoom WRITE setZoom NOTIFY zoomChanged)

public:
    explicit CanvasView(QQuickItem *parent = 0);
    virtual ~CanvasView();

    Canvas *canvas() const { return m_canvas; }
    void setCanvas(Canvas *canvas);

    CanvasFrame *canvasFrame() const { return m_canvasFrame; }

    qreal zoom() const { return imageItem->scale(); }
    void setZoom(qreal zoom);

    void dragTo(const QPointF &pos);
    void dragTo(qreal x, qreal y) { dragTo(QPointF(x, y)); }

    void dragBy(const QPointF &dpos) { dragTo(canvasFrame()->position() + dpos); }
    void dragBy(qreal dx, qreal dy) { dragBy(QPointF(dx, dy)); }

signals:
    void canvasChanged();
    void zoomChanged();

public slots:
    void refreshCanvas();

protected:
    virtual void componentComplete();

private:
    Canvas *m_canvas;
    CanvasFrame *const m_canvasFrame;

    CanvasProvider *canvasProvider;
    QQuickItem *imageItem;

    friend class CanvasFrame;
};

#endif // CANVASVIEW_HPP
