#ifndef MAIN_PROJECT_HPP
#define MAIN_PROJECT_HPP


#include <QObject>

#include "core/Canvas.hpp"


class Project : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Canvas *canvas READ canvas NOTIFY canvasChanged)

public:
    explicit Project(QObject *parent = nullptr);

    Canvas *canvas() { return &m_canvas; }

signals:
    void canvasChanged();

private:
    Canvas m_canvas;
};


#endif // MAIN_PROJECT_HPP
