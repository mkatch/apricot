#ifndef CANVAS_HPP
#define CANVAS_HPP

#include <QObject>
#include <QPixmap>

class Canvas : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int width READ width)
    Q_PROPERTY(int height READ height)
    Q_PROPERTY(QSize size READ size)

public:
    Canvas(const QString& fileName, QObject *parent = nullptr);

    int width() const { return pixmap.width(); }
    int height() const { return pixmap.height(); }
    QSize size() const { return pixmap.size(); }

    QPixmap toPixmap() const { return pixmap; }

private:
    QPixmap pixmap;
};

#endif // CANVAS_HPP
