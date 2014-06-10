#ifndef CORE_PAINTER_HPP
#define CORE_PAINTER_HPP

#include <QPainter>
#include <QString>

class Canvas;

class Painter
{
public:
    explicit Painter(Canvas &canvas);

    const QBrush &brush() const;
    const QPen &pen() const;
    QPainter::CompositionMode compositionMode() const;

    void setBrush(const QBrush &brush);
    void setPen(const QPen &pen);
    void setCompositionMode(QPainter::CompositionMode mode);

    void drawImage(QString fileName);
    void drawPoint(const QPoint &p);
    void drawPoint(int x, int y);
    void drawLine(const QPoint &p1, const QPoint &p2);
    void drawLine(int x1, int y1, int x2, int y2);
    void drawRect(const QRect &rect);
    void drawRect(int x, int y, int width, int height);
    void drawEllipse(const QRect &rect);
    void drawEllipse(const QPoint &center, int rx, int ry);
    void drawCanvas(const QPoint &p, const Canvas &canvas, const QRect &sourceRect);

    const QRect &boundingBox() const;

private:
    QPainter painter;

    QRect m_boundingBox;
};

inline const QBrush &Painter::brush() const
{
    return painter.brush();
}

inline const QPen &Painter::pen() const
{
    return painter.pen();
}

inline QPainter::CompositionMode Painter::compositionMode() const
{
    return painter.compositionMode();
}

inline void Painter::setBrush(const QBrush &brush)
{
    painter.setBrush(brush);
}

inline void Painter::setPen(const QPen &pen)
{
    painter.setPen(pen);
}

inline void Painter::setCompositionMode(QPainter::CompositionMode mode)
{
    painter.setCompositionMode(mode);
}

inline void Painter::drawPoint(int x, int y)
{
    drawPoint(QPoint(x, y));
}

inline void Painter::drawLine(int x1, int y1, int x2, int y2)
{
    drawLine(QPoint(x1, y1), QPoint(x2, y2));
}

inline void Painter::drawRect(int x, int y, int width, int height)
{
    drawRect(QRect(x, y, width, height));
}

inline void Painter::drawEllipse(const QPoint &center, int rx, int ry)
{
    drawEllipse(QRect(center.x() - rx, center.y() - ry, 2 * rx, 2 * ry));
}

inline const QRect &Painter::boundingBox() const
{
    return m_boundingBox;
}

#endif // CORE_PAINTER_HPP
