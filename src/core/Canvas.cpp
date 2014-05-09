#include "Canvas.hpp"

Canvas::Canvas(const QString &fileName, QObject *parent) :
    QObject(parent),
    pixmap(fileName)
{
    // Do nothing
}
