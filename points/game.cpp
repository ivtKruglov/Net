#include "game.h"
#include "ui_game.h"

Game::Game(QWidget *parent):
    QDialog(parent),
    ui(new Ui::Game)
{
    ui->setupUi(this);
    this->setFixedSize(440, 460);

    // Следует узнать внешний ip
    setAddress();
    socket.bind(address, port);

    lines.init_h();
    lines.init_v();
    interpoints = interection();
    permit = false;
    preData = false;
    score_1 = 0;
    score_2 = 0;
    ui->label_5->setText(QString::number(score_1));
    ui->label_6->setText(QString::number(score_2));

    connect(this, SIGNAL(mainwin()), this, SLOT(close()));
    connect(this, SIGNAL(clicked()), this, SLOT(repaint()));
    connect(&socket, SIGNAL(readyRead()), this, SLOT(socketReady()));
}

Game::~Game()
{
    delete ui;
}

void Game::socketReady()
{
    QPoint p_2;
    QString x;
    QString y;
    int sep;      // Разделитель
    QVector<QVector<QPoint>> listfs;

    data.resize(quint16(socket.pendingDatagramSize()));
    status = socket.readDatagram(data.data(), data.size(), &serveraddress, &serverport);
    while (status == -1)
        status = socket.readDatagram(data.data(), data.size(), &serveraddress, &serverport);
    if (preData)
    {
        QString datastr = data.data();
        if (datastr != "leave")
        {
            permit = true;
            ui->label_4->setText("Ваш ход");

            sep = data.indexOf('|');
            for (int i = 0; i < sep; ++i)
            {
                x.append(data.data()[i]);
            }
            for (int i = sep+1; i < data.size(); ++i)
            {
                y.append(data.data()[i]);
            }

            p_2.setX(x.toInt());
            p_2.setY(y.toInt());
            if (not inArr(p_2, player_2))
                player_2.push_back(p_2);
            searchClosedPoints();
            ui->label_6->setText(QString::number(score_2));
            sortClosedPoints();
            repaint();

            if (isFill())
            {
                QMessageBox msgWin;
                QString winner;
                if (score_1 > score_2)
                    msgWin.setInformativeText("Вы выиграли.");
                else if (score_2 > score_1)
                    msgWin.setInformativeText("Вы проиграли.");
                else
                    msgWin.setInformativeText("Ничья.");
                QPushButton *button = msgWin.addButton("Выйти из игры", QMessageBox::ActionRole);
                msgWin.exec();
                if (msgWin.clickedButton() == button)
                    emit mainwin();
            }
        }
        else
        {
            QMessageBox msgWin;
            msgWin.setInformativeText("Игрок " + ui->label_3->text() + " покинул игру.");
            msgWin.setIcon(QMessageBox::Warning);
            QPushButton *button = msgWin.addButton("Выйти из игры", QMessageBox::ActionRole);
            msgWin.exec();
            if (msgWin.clickedButton() == button)
                emit mainwin();
        }
    }
    else
    {
        QVector<QString> data_arr;
        QString item;
        for (int i = 0; i < data.size(); ++i)
        {
            if (data.at(i) != '|')
                item.append(data.at(i));
            else
            {
                data_arr.append(item);
                item.clear();
            }
        }
        nickname2 = data_arr.value(0);
        ui->label_3->setText(nickname2);
        if (data_arr.value(1) == "1")
        {
            colors.append(Qt::red);
            colors.append(Qt::green);
        }
        else
        {
            colors.append(Qt::green);
            colors.append(Qt::red);
        }
        if (data_arr.value(2) == "1")
        {
            permit = true;
            ui->label_4->setText("Ваш ход");
        }
        else
            ui->label_4->setText("Ход 2 игрока");
        preData = true;
    }
    data.clear();
}

QVector<QPoint> Game::interection()
{
    QVector<QPoint> interpoints;

    for (int i = 0; i < 20; ++i)
    {
        for (int j = 0; j < 20; ++j)
        {
            for (int x = lines.getLines_h()[i].x1(); x <= lines.getLines_h()[i].x2(); ++x)
            {
                for (int y = lines.getLines_v()[j].y1(); y <= lines.getLines_v()[j].y2(); ++y)
                {
                    if (x == lines.getLines_v()[j].x1() && y == lines.getLines_h()[i].y1())
                    {
                        interpoints.push_back(QPoint(x, y));
                        break;
                    }
                }
            }
        }
    }
    return interpoints;
}

bool Game::isExist(QPoint &p)
{
    bool res = false;
    for (QPoint interp:interpoints)
    {
        if (interp.x()-5 <= p.x() && p.x() <= interp.x()+5 && interp.y()-5 <= p.y() && p.y() <= interp.y()+5)
        {
            p.setX(interp.x());
            p.setY(interp.y());
            res = true;
        }
    }
    return res;
}

QVector<QPoint> Game::searchNeighbors(const QPoint &currp)
{
    // Считается, что точка является окруженной, если соседние точки по вертикали и горизонтали заняты.
    // Поэтому для проверки можно вернуть только их, не затрагивая соседей по горизонтали.

    QPoint nearp;
    QVector<QPoint> npoints;
    for (int x = currp.x()-18; x <= currp.x()+18; x += 18)
    {
        for (int y = currp.y()-18; y <= currp.y()+18; y += 18)
        {
            if ((currp.x() == x && currp.y() != y) || (currp.x() != x && currp.y() == y))
            {
                nearp.setX(x);
                nearp.setY(y);
                npoints.append(nearp);
            }
        }
    }
    return npoints;
}

QVector<QPoint> Game::searchNearPoints(const QPoint &currp)
{
    QPoint nearp;
    QVector<QPoint> npoints;
    for (int x = currp.x()-18; x <= currp.x()+18; x += 18)
    {
        for (int y = currp.y()-18; y <= currp.y()+18; y += 18)
        {
            if (currp.x() != x || currp.y() != y)
            {
                nearp.setX(x);
                nearp.setY(y);
                npoints.append(nearp);
            }
        }
    }
    return npoints;
}

void Game::searchClosedPoints()
{
    QPoint p;
    QVector<QPoint> nps;
    int result;
    bool check;
    for (QPoint ip:interpoints)
    {
        check = true;
        result = 0;

        p.setX(ip.x());
        p.setY(ip.y());

        for (QVector<QPoint> regn:closedp)
        {
            if (inArr(p, regn))
                check = false;
        }
        for (QVector<QPoint> sur:surroundedp)
        {
            if (inArr(p, sur))
                check = false;
        }
        if (check)
        {
            if (inArr(p, player_2))
            {
                nps = searchNearPoints(p);
                for (QPoint nr:nps)
                {
                    if (nr.y() > p.y() && !inArr(nr, player_2) && result != -1)
                    {
                        friends.append(nr);
                        result = checkPoints(friends, player_2, interpoints.first().x(), p.y(), interpoints.last().x(), interpoints.last().y());
                        if (result == 1)
                        {
                            QVector<QPoint> r;
                            surroundedp.append(friends);
                            if (isSurr(player_1) > 0)
                            {
                                score_2 += isSurr(player_1);
                                for (QPoint surrp:surroundedp.last())
                                {
                                    nps = searchNeighbors(surrp);
                                    for (QPoint clp:nps)
                                    {
                                        if (!inArr(clp, r) && inArr(clp, player_2) && !inArrs(clp, surroundedp))
                                            r.append(clp);
                                    }
                                }
                                closedp.append(r);
                            }
                            else
                                surroundedp.removeLast();
                            friends.clear();
                            viewedps.clear();
                            break;
                        }
                    }
                }
            }
            else if (inArr(p, player_1))
            {
                nps = searchNearPoints(p);
                for (QPoint nr:nps)
                {
                    if (nr.y() > p.y() && !inArr(nr, player_1) && result != -1)
                    {
                        friends.append(nr);
                        result = checkPoints(friends, player_1, interpoints.first().x(), p.y(), interpoints.last().x(), interpoints.last().y());
                        if (result == 1)
                        {
                            QVector<QPoint> r;
                            surroundedp.append(friends);
                            if (isSurr(player_2) > 0)
                            {
                                score_1 += isSurr(player_2);
                                for (QPoint surrp:surroundedp.last())
                                {
                                    nps = searchNeighbors(surrp);
                                    for (QPoint clp:nps)
                                    {
                                        if (!inArr(clp, r) && inArr(clp, player_1) && !inArrs(clp, surroundedp))
                                            r.append(clp);
                                    }
                                }
                                closedp.append(r);
                            }
                            else
                                surroundedp.removeLast();
                            friends.clear();
                            viewedps.clear();
                            break;
                        }
                    }
                }
            }
        }
    }
}

int Game::checkPoints(QVector<QPoint> &nbs, QVector<QPoint> player, const int &minx, const int &miny, const int &maxx, const int &maxy)
{
    // 0 - есть новые непроверенные соседи
    // 1 - точка окружена врагами и проверенными точками -> контур замкнут
    // -1 - точка рядом с границей -> контур не замкнут
    int result = 1;
    for (QPoint f:friends)
    {
        QVector<QPoint> nps = searchNeighbors(f);
        for (QPoint np:nps)
        {
            if (!inArr(np, viewedps) && (inArrs(np, surroundedp) || !inArr(np, player)))
            {
                if (np.x() != minx && np.x() != maxx && np.y() != miny && np.y() != maxy)
                {
                    if (!inArr(np, friends))
                        friends.append(np);
                    if (!inArr(np, viewedps))
                        viewedps.append(nbs);
                    result = 0;
                }
                else
                {
                    result = -1;
                    friends.clear();
                    viewedps.clear();
                    break;
                }
            }

        }
        if (result == -1)
            break;
    }
    if (result != 0)
        return result;
    else
        return checkPoints(friends, player, minx, miny, maxx, maxy);
}

void Game::sortClosedPoints()
{
    QVector<QPoint> nearpoints;
    QVector<QPoint> viewedpoints;
    for (QVector<QPoint> arr:closedp)
    {
        for (int i = 0; i < arr.size()-1; ++i)
        {
            if (abs(arr.value(i).x()-arr.value(i+1).x()) > 18 || abs(arr.value(i).y()-arr.value(i+1).y()) > 18)
            {
                qDebug() << i;
                nearpoints = searchNearPoints(arr.value(i));
                for (QPoint np:nearpoints)
                {
                    if (inArr(np, arr) && !inArr(np, viewedpoints))
                    {
                        QPoint temp = arr.value(i+1);
                        arr.value(i+1) = np;
                        for (QPoint p:arr)
                        {
                            if (p == np)
                                p = temp;
                        }
                        break;
                    }
                }
            }
            viewedpoints.append(closedp.value(i));
        }
    }
    nearpoints.clear();
    viewedpoints.clear();
}

int Game::isSurr(QVector<QPoint> player)
{
    int count = 0;
    for (QPoint last:surroundedp.last())
    {
        if (inArr(last, player))
            count += 1;
    }
    return count;
}

bool Game::inArr(QPoint &p, QVector<QPoint> &arr)
{
    bool res = false;
    for (QPoint vp: arr)
    {
        if (vp == p)
        {
            res = true;
            break;
        }
        else
            continue;
    }
    return res;
}

bool Game::inArrs(QPoint &p, QVector<QVector<QPoint> > &arrs)
{
    bool res = false;
    for (QVector<QPoint> arr:arrs)
        if (inArr(p, arr))
            res = true;
    return res;
}

bool Game::isFill()
{
    bool result = true;
    for (QPoint irp:interpoints)
    {
        if (!inArr(irp, player_1) && !inArr(irp, player_2))
            result = false;
    }
    return result;
}

void Game::newNickname(const QString &newnick)
{
    nickname = newnick;
}

void Game::newServerAddress(const QString &ip)
{
    serveraddress = QHostAddress(ip);
}

void Game::newServerPort(const quint16 &port)
{
    serverport = port;
}

void Game::setAddress()
{
    QTcpSocket socket;
    socket.connectToHost("8.8.8.8", 53);
    if (socket.waitForConnected())
        address = socket.localAddress();
    socket.disconnect();
}

void Game::on_pushButton_clicked()
{
    QMessageBox msgWin;
    msgWin.setInformativeText("Вы уверены, что хотите покинуть игру?");
    msgWin.setIcon(QMessageBox::Question);
    QPushButton *yes = msgWin.addButton("Да", QMessageBox::ActionRole);
    QPushButton *no = msgWin.addButton("Нет", QMessageBox::ActionRole);
    msgWin.exec();
    if (msgWin.clickedButton() == yes)
    {
        // Следует добавить таймер для отслеживания времени бездействия игрока, чтобы
        // предусмотреть неудачную отправку пакета и по достижению
        // какого-то определенного времени бездействия уведомить второго игрока о том, что первый игрок,
        // возможно, покинул игру, но сервер не получит данные об этом.
        socket.writeDatagram("gameover", serveraddress, serverport);
        emit mainwin();
    }
    else if (msgWin.clickedButton() == no)
    {
        delete yes;
        delete no;
        msgWin.close();
    }
}

void Game::showEvent(QShowEvent *)
{
    ui->label_2->setText(nickname);
    data.append(nickname);
    status = socket.writeDatagram(data.data(), serveraddress, serverport);
    while (status == -1)
        status = socket.writeDatagram(data.data(), serveraddress, serverport);
    data.clear();
}

void Game::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QPoint Pos = QCursor::pos();
        Pos = QWidget::mapFromGlobal(Pos);
        if (isExist(Pos) && permit && !inArr(Pos, player_2) && !inArr(Pos, player_1))
        {
            permit = false;
            data.append(QString::number(Pos.x()));
            data.append("|");
            data.append(QString::number(Pos.y()));
            status = socket.writeDatagram(data.data(), serveraddress, serverport);
            while (status == -1)
                status = socket.writeDatagram(data.data(), serveraddress, serverport);
            data.clear();
            player_1.push_back(Pos);
            searchClosedPoints();
            ui->label_5->setText(QString::number(score_1));
            repaint();

            if (isFill())
            {
                QMessageBox msgWin;
                QString winner;
                if (score_1 > score_2)
                    msgWin.setInformativeText("Вы выиграли.");
                else if (score_2 > score_1)
                    msgWin.setInformativeText("Вы проиграли.");
                else
                    msgWin.setInformativeText("Ничья.");
                QPushButton *button = msgWin.addButton("Выйти из игры", QMessageBox::ActionRole);
                msgWin.exec();
                if (msgWin.clickedButton() == button)
                    emit mainwin();
            }
            else
            {
                emit clicked();
                ui->label_4->setText("Ход 2 игрока");
            }
        }
    }
}

void Game::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    draw.draw_lines(painter, lines);
    for (auto arr:closedp)
        draw.draw_closed_lines(painter, arr, colors.value(0));
    draw.draw_ellispe(painter, player_1, colors.value(0));
    draw.draw_ellispe(painter, player_2, colors.value(1));
}
