#ifndef CORE_PAINTER_HPP
#define CORE_PAINTER_HPP

#include <QPainter>
#include <QString>
#include <memory>

class Canvas;

class Painter
{
public:
    explicit Painter(Canvas &canvas);

    void drawImage(QString fileName);
    void drawPoint(int x, int y);
    void drawPoint(const QPoint &position);

    const QRect &boundingBox() const;
    const QColor &brushColor() const;
    int brushSize() const;

    void setBrushColor(const QColor &color);
    void setBrushSize(int size);

private:
    std::shared_ptr<QPainter> painter;

    QRect m_boundingBox;

    QColor m_brushColor;
    int m_brushSize;
};

inline void Painter::drawPoint(const QPoint &position)
{
    drawPoint(position.x(), position.y());
}

inline const QRect &Painter::boundingBox() const
{
    return m_boundingBox;
}

inline const QColor &Painter::brushColor() const
{
    return m_brushColor;
}

inline int Painter::brushSize() const
{
    return m_brushSize;
}

inline void Painter::setBrushColor(const QColor &color)
{
    m_brushColor = color;
}

inline void Painter::setBrushSize(int size)
{
    m_brushSize = size;
}

#endif // CORE_PAINTER_HPP
