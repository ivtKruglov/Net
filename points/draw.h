#ifndef DRAW_H
#define DRAW_H

#include "lines.h"
#include <QPainter>

class Draw
{
public:
    Draw();
    void draw_ellispe(QPainter &qp, QVector<QPoint> &points, const QColor &color);
    void draw_lines(QPainter &qp, Lines &lines);
    void draw_closed_lines(QPainter &qp, QVector<QPoint> &closedp, const QColor &color);
};

#endif // DRAW_H
