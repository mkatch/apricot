#ifndef CORE_PAINTER_HPP
#define CORE_PAINTER_HPP

#include <QString>

class Canvas;

class Painter
{
public:
    explicit Painter(Canvas *canvas);
    Painter(const Painter& other);

    void load(QString fileName);

private:
    Canvas *canvas;
};

#endif // CORE_PAINTER_HPP
