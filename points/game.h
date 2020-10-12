#ifndef GAME_H
#define GAME_H

#include "draw.h"
#include <QUdpSocket>
#include <QHostInfo>
#include <QDialog>
#include <QString>
#include <QMouseEvent>
#include <QCursor>
#include <QDebug>
#include <QMessageBox>

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

    QVector<QPoint> interection();                      // Поиск точек пересекающихся линий
    bool isExist(QPoint &p);                            // Проверка нахождения точки на пересечении линий
    QVector<QPoint> searchNeighbors(const QPoint &currp);    // Поиск соседних точек
    QVector<QPoint> searchNearPoints(const QPoint &currp);   // Поиск точек, находящихся в шаге от текущей
    void searchClosedPoints();                          // Поиск окружающих точек

    // Проверка свободных точек и точек, не являющихся граничными, на нахождение на границе или
    // в замкнутой области
    int checkPoints(QVector<QPoint> &nbs, QVector<QPoint> player,
                    const int &minx, const int &miny, const int &maxx, const int &maxy);

    void sortClosedPoints();                            // Сортировка окружающих точек

    // Подсчет количества точек определенного игрока player в замкнутой области
    int isSurr(QVector<QPoint> player);

    bool inArr(QPoint &p, QVector<QPoint> &arr);        // Проверка существования точки в массиве

    // Проверка существования точки в одном из массивов массива
    bool inArrs(QPoint &p, QVector<QVector<QPoint>> &arrs);
    bool isFill();

    void newNickname(const QString &newnick);           // Задать новый ник клиенту
    void newServerAddress(const QString &ip);           // Задать адрес запущенного сервера
    void newServerPort(const quint16 &port);            // Задать порт запущенного сервера

    // Узнать адрес данного клиента и запомнить, чтобы связать с портом этого же клиента через сокет
    void setAddress();
    void clearGame();                                   // Очистить окно и задать значения, как при инициализации

signals:
    void mainwin();                                     // Сигнал для открытия mainwindow
    void clicked();                                     // Сигнал для перерисовки

private slots:
    void showEvent(QShowEvent *);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *);
    void closeEvent(QCloseEvent *);
    void socketReady();
    void on_pushButton_clicked();                 // Завершить игру

private:
    Ui::Game *ui;

    QUdpSocket socket;                            // Сокет клиента
    QByteArray data;                              // Данные, отправляемые по сокету
    QHostAddress address;                         // Адрес клиента
    quint16 port;                                 // Порт клиента
    QHostAddress serveraddress;                   // Адрес сервера
    quint16 serverport;                           // Порт сервера
    QString nickname;                             // Имя данного клиента
    QString nickname2;                            // Имя другого клиента

    QVector<QPoint> player_1;                     // Точки первого игрока
    QVector<QPoint> player_2;                     // Точки второго игрока
    QVector<QColor> colors;                       // Цвета точек игроков
    Lines lines;                                  // Объект класса для рисования линий
    Draw draw;                                    // Объект класса для рисования других фигур
    QVector<QPoint> interpoints;                  // Точки пересечений линий
    QVector<QVector<QPoint>> closedp;             // Точки, замыкающие контур
    QVector<QVector<QPoint>> surroundedp;         // Окруженные точки
    QVector<QPoint> viewedps;                     // Проверенные точки
    QVector<QPoint> friends;                      // Соседи точки, находящейся внутри области
    bool permit;                                  // Разрешение на ход
    bool preData;                                 // Готовность к игре
    bool skipEvent;                               // Переменная для игнорирования или подтверждения события closeEvent
    int score_1;                                  // Счет 1 игрока
    int score_2;                                  // Счет 2 игрока
    qint64 status;                                // Результат отправки или чтения пакета
};

#endif // GAME_H
