#ifndef PROJECT_HPP
#define PROJECT_HPP

#include <QObject>

#include "core/canvas.hpp"

class Project : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Canvas *canvas READ canvas NOTIFY canvasChanged)

public:
    explicit Project(QObject *parent = 0);

    Canvas *canvas() { return &m_canvas; }

signals:
    void canvasChanged();

private:
    Canvas m_canvas;
};

#endif // PROJECT_HPP
