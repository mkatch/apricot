#ifndef CORE_CANVAS_HPP
#define CORE_CANVAS_HPP


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

    const QPixmap &toPixmap() const { return pixmap; }

private:
    QPixmap pixmap;
};


#endif // CORE_CANVAS_HPP
