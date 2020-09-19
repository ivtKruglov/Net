#ifndef DRAW_H
#define DRAW_H

#include "lines.h"
#include <QPainter>

class Draw
{
public:
    Draw();
    void draw_ellispe(QPainter *qp, QVector<QPoint> *points);
    void draw_lines(QPainter *qp, Lines *lines);
    void draw_closed_lines(QPainter *qp, QVector<QPoint> *closedp);
};

#endif // DRAW_H
