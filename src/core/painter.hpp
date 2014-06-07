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

private:
    std::shared_ptr<QPainter> painter;
};

#endif // CORE_PAINTER_HPP
