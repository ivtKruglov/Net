#ifndef LINES_H
#define LINES_H
#include <QVector>
#include <QLine>

class Lines : public QLine
{
private:
    QVector<QLine> *lines_h;
    QVector<QLine> *lines_v;

public:
    Lines();
    QVector <QLine> getLines_h();
    QVector <QLine> getLines_v();
    void addLine(QVector <QLine> *lines, QLine line);
    int getSize(QVector<QLine> lines);
    void init_h();
    void init_v();
};

#endif // LINES_H
