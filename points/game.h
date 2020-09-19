#ifndef GAME_H
#define GAME_H

#include "draw.h"
#include <QUdpSocket>
#include <QDialog>
#include <QVector>
#include <QString>
#include <QMouseEvent>
#include <QCursor>

namespace Ui
{
class Game;
}

class Game : public QDialog
{
    Q_OBJECT

public:
    explicit Game(QWidget *parent = nullptr);
    ~Game();

    QVector<QPoint> interection();                // Поиск точек пересекающихся линий
    bool isExist(QPoint *p);                      // Проверка нахождения точки на пересечении линий
    bool findnbs(QPoint *p, QPoint *curr_p);      // Поиск соседних точек
    bool inArr(QPoint *p, QVector<QPoint> *arr);  // Проверка существования точки в массиве
    QVector<QPoint> findClosed();                 // Поиск точек, замыкающих контур

    QUdpSocket *socket;                           // Сокет клиента
    QByteArray *data;                             // Данные, отправляемые по сокету
    QHostAddress *address;                        // Адрес клиента
    QHostAddress *serveraddress;                  // Адрес сервера
    quint16 *port;                                // Порт клиента
    quint16 *serverport;                          // Порт сервера

signals:
    void mainwin();                               // Сигнал для открытия mainwindow
    void clicked();                               // Сигнал для перерисовки

private slots:
    void on_pushButton_clicked();                 // Завершить игру

public slots:
    void socketReady();                           // Действия при поступлении дейтаграммы клиенту

protected slots:
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::Game *ui;

    QVector<QPoint> *player_1;                    // Точки первого игрока
    QVector<QPoint> *player_2;                    // Точки второго игрока
    Lines *lines;
    Draw *draw;
    QVector<QPoint> *interpoints;                 // Точки пересечений линий
    QVector<QPoint> *closedpoints;                // Точки, замыкающие контур
    bool *permit;                                 // Разрешение на ход

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // GAME_H
