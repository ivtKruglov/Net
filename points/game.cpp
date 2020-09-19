#include "game.h"
#include "ui_game.h"

Game::Game(QWidget *parent):
    QDialog(parent),
    ui(new Ui::Game)
{
    ui->setupUi(this);
    this->setFixedSize(440, 460);

    socket = new QUdpSocket(this);
    data = new QByteArray();
    address = new QHostAddress();
    serveraddress = new QHostAddress();
    port = new quint16();
    serverport = new quint16();

    socket->bind(*address, *port);
    data->append("");

    player_1 = new QVector<QPoint>();
    player_2 = new QVector<QPoint>();
    lines = new Lines();
    draw = new Draw();
    interpoints = new QVector<QPoint>();
    closedpoints = new QVector<QPoint>();
    permit = new bool();
    lines->init_h();
    lines->init_v();
    *interpoints = interection();

    connect(this, SIGNAL(clicked()), this, SLOT(repaint()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(socketReady()));
}


Game::~Game()
{
    delete ui;
}


void Game::socketReady()
{
    QPoint *p_2 = new QPoint();
    QString *x = new QString();
    QString *y = new QString();
    int *sep = new int();      // Разделитель

    data->resize(quint16(socket->pendingDatagramSize()));
    socket->readDatagram(data->data(), data->size(), serveraddress, serverport);
    *sep = data->indexOf('|');

    for (int i = 0; i < *sep; ++i)
    {
        x->append(data->data()[i]);
    }
    for (int i = *sep+1; i < data->size(); ++i)
    {
        y->append(data->data()[i]);
    }

    p_2->setX(x->toInt());
    p_2->setY(y->toInt());
    if (not inArr(p_2, player_2))
        player_2->push_back(*p_2);
    data->clear();
    repaint();
    ui->label_4->setText("Ваш ход");
}


QVector<QPoint> Game::interection()
{
    QVector<QPoint> *interpoints = new QVector<QPoint>;

    for (int i = 0; i < 20; ++i)
    {
        for (int j = 0; j < 20; ++j)
        {
            for (int x = lines->getLines_h()[i].x1(); x <= lines->getLines_h()[i].x2(); ++x)
            {
                for (int y = lines->getLines_v()[j].y1(); y <= lines->getLines_v()[j].y2(); ++y)
                {
                    if (x == lines->getLines_v()[j].x1() && y == lines->getLines_h()[i].y1())
                    {
                        interpoints->push_back(QPoint(x, y));
                        break;
                    }
                }
            }
        }
    }
    return *interpoints;
}


bool Game::isExist(QPoint *p)
{
    bool res = false;
    for (QPoint interp:*interpoints)
    {
        if (interp.x()-5 <= p->x() && p->x() <= interp.x()+5 && interp.y()-5 <= p->y() && p->y() <= interp.y()+5)
        {
            p->setX(interp.x());
            p->setY(interp.y());
            res = true;
        }
    }
    return res;
}


bool Game::findnbs(QPoint *p, QPoint *curr_p)
{
    QPoint *nb = new QPoint;  // Соседняя точка
    bool res = false;
    for (int x = curr_p->x()-18; x <= curr_p->x()+18; x += 18)
    {
        for (int y = curr_p->y()-18; y <= curr_p->y()+18; y += 18)
        {
            nb->setX(x);
            nb->setY(y);
            if (nb->x() == p->x() && nb->y() == p->y())
            {
                res = true;
            }
        }
    }
    return res;
}


bool Game::inArr(QPoint *p, QVector<QPoint> *arr)
{
    bool res = false;
    for (QPoint vp: *arr)
    {
        if (vp == *p)
            res = true;
    }
    return res;
}


QVector<QPoint> Game::findClosed()
{
    QPoint *beginPoint = new QPoint;                      // Начальная точка
    QPoint *currentPoint = new QPoint;                    // Текущая точка
    QVector<QPoint> *closedPoints = new QVector<QPoint>;  // Точки, замыкающие контур
    QVector<QPoint> *viewedPoints = new QVector<QPoint>;  // Рассмотренные точки

    bool closed = false;
    *beginPoint = player_1->first();
    viewedPoints->push_back(*beginPoint);
    *currentPoint = *beginPoint;

    // Поиск замкнутого контура
    for (QPoint point: *player_1)
    {
        if (findnbs(&point, currentPoint))
        {
            *currentPoint = point;
            if (findnbs(beginPoint, currentPoint) && viewedPoints->size() > 2)
            {
                closed = true;
            }
        }

        if (not inArr(&point, viewedPoints))
        {
            viewedPoints->push_back(point);
        }
    }

    if (closed)
        for (QPoint vp: *viewedPoints)
        {
            closedPoints->push_back(vp);
        }

    return *closedPoints;
}


void Game::on_pushButton_clicked()
{
    emit mainwin();
    closedpoints->clear();
    player_1->clear();
    player_2->clear();
    this->close();
}


void Game::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QPoint Pos = QCursor::pos();
        Pos = QWidget::mapFromGlobal(Pos);
        if (isExist(&Pos))
        {
            data->append(QString::number(Pos.x()));
            data->append("|");
            data->append(QString::number(Pos.y()));
            socket->writeDatagram(data->data(), QHostAddress::LocalHost, 1000);
            data->clear();
            player_1->push_back(Pos);
            *closedpoints = findClosed();
            emit clicked();
            ui->label_4->setText("Ход 2 игрока");
        }
    }
}


void Game::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    draw->draw_lines(&painter, lines);
    draw->draw_closed_lines(&painter, closedpoints);
    draw->draw_ellispe(&painter, player_1);
    draw->draw_ellispe(&painter, player_2);
}
