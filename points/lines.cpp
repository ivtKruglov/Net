#include "lines.h"

Lines::Lines()
{
    lines_h = new QVector<QLine>;
    lines_v = new QVector<QLine>;
}


QVector<QLine> Lines::getLines_h()
{
    return *lines_h;
}


QVector<QLine> Lines::getLines_v()
{
    return *lines_v;
}


void Lines::addLine(QVector<QLine> *lines, QLine line)
{
    lines->push_back(line);
}


int Lines::getSize(QVector<QLine> lines)
{
    return lines.size();
}


void Lines::init_h()
{
    int x = 40, y = 40, x_2 = 400, y_2 = 40;
    lines_h->push_back(QLine(x, y, x_2, y_2));
    for (int i = 0; i < 20; ++i)
    {
        y += 18;
        y_2 += 18;
        lines_h->push_back(QLine(x, y, x_2, y_2));
    }
}


void Lines::init_v()
{
    int x = 40, y = 40, x_2 = 40, y_2 = 400;
    lines_v->push_back(QLine(x, y, x_2, y_2));
    for (int i = 0; i < 20; ++i)
    {
        x += 18;
        x_2 += 18;
        lines_v->push_back(QLine(x, y, x_2, y_2));
    }
}
