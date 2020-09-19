#include "draw.h"

Draw::Draw(){}

void Draw::draw_ellispe(QPainter *qp, QVector<QPoint> *points)
{
    qp->setPen(QPen(Qt::black, 1, Qt::SolidLine));
    qp->setBrush(QBrush(Qt::red, Qt::SolidPattern));
    for (int i = 0; i < points->size(); ++i)
    {
        qp->drawEllipse(points->value(i), 5, 5);
    }
}

void Draw::draw_lines(QPainter *qp, Lines *lines)
{
    qp->setPen(QPen(Qt::gray, 1, Qt::SolidLine));
    qp->drawLines(lines->getLines_h());
    qp->drawLines(lines->getLines_v());
}


void Draw::draw_closed_lines(QPainter *qp, QVector<QPoint> *closedp)
{
    qp->setPen(QPen(Qt::red, 2, Qt::SolidLine));
    for (int i = 0; i < closedp->size()-1; ++i)
    {
        qp->drawLine(closedp->value(i).x(), closedp->value(i).y(), closedp->value(i+1).x(), closedp->value(i+1).y());
    }
    if (closedp->size() > 0)
        qp->drawLine(closedp->value(0).x(), closedp->value(0).y(), closedp->last().x(), closedp->last().y());
}
