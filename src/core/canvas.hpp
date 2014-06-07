#ifndef CORE_CANVAS_HPP
#define CORE_CANVAS_HPP

#include <QPixmap>

class Canvas {
public:
    Canvas();
    Canvas(int width, int height);
    Canvas(const QSize &size);
    Canvas(const Canvas &canvas);
    Canvas(const QPixmap &pixmap);

    void fill(const QColor &color = Qt::white);
    void setSize(int width, int height);
    void setSize(const QSize &size);

    int width() const {
        return m_pixmap.width();
    }

    int height() const {
        return m_pixmap.height();
    }

    QSize size() const {
        return m_pixmap.size();
    }

    const QPixmap &pixmap() const {
        return m_pixmap;
    }

private:
    QPixmap m_pixmap;

    friend class Painter;
};

#endif // CORE_CANVAS_HPP
