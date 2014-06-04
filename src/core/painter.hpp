#ifndef CORE_PAINTER_HPP
#define CORE_PAINTER_HPP

#include <QString>

#include "canvas.hpp" // TODO: remove

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
