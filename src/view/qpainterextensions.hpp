#ifndef VIEW_QPAINTEREXTENSIONS_HPP
#define VIEW_QPAINTEREXTENSIONS_HPP

#include <QPainter>

#include <ApricotCore>
#include <ApricotModel>

class QPainterExtensions
{
public:
    QPainterExtensions(QPainter *painter);

    void drawAnimationFrame(const QRect &targetRect, const AnimationFrame *frame);
    void drawAnimationFrame(int x, int y, const AnimationFrame *frame);
    void drawAnimationFrame(int x, int y, int width, int height, const AnimationFrame * frame);

private:
    QPainter *painter;
};

#endif // QPAINTEREXTENSIONS_HPP
