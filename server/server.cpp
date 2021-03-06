#include "server.h"

Server::Server(){}

Server::~Server(){}

void Server::startServer()
{
    setAddress();
    quint16 serverport = 1000;                                 // Порт сервера
    socket.bind(serveraddr, serverport);

    qDebug() << "Сервер запущен";
    qDebug() << "IP-адрес сервера: " << socket.localAddress().toString();
    qDebug() << "Порт сервера: " << serverport;

    connect(&socket, SIGNAL(readyRead()), this, SLOT(socketReady()));
}

void Server::socketReady()
{
    QString datastr;
    data.resize(quint16(socket.pendingDatagramSize()));
    status = socket.readDatagram(data.data(), data.size(), &sender, &senderport);
    while (status == -1)
        status = socket.readDatagram(data.data(), data.size(), &sender, &senderport);
    datastr = data.data();
    // Если пришел пакет от не играющего (лишнего) клиента, то удалить содержимое
    if (senderports.size() == 2 && (!inArr(senderport, senderports) || !inArr(sender, senders)))
        data.clear();
    else if (senderports.size() < 2 && (!inArr(senderport, senderports) || !inArr(sender, senders)) && datastr != "leave")
    {
        bool rand = generateRand();
        QVector<QString> colors;
        QVector<QString> turns;
        if (rand)
        {
            colors.append(QString::number(rand));
            colors.append(QString::number(false));
        }
        else
        {
            colors.append(QString::number(rand));
            colors.append(QString::number(true));
        }
        rand = generateRand();
        if (rand)
        {
            turns.append(QString::number(rand));
            turns.append(QString::number(false));
        }
        else
        {
            turns.append(QString::number(rand));
            turns.append(QString::number(true));
        }
        senderports.append(senderport);
        senders.append(sender);
        qDebug() << "Подключился игрок " << data.data();
        nicknames.append(data.data());
        data.clear();
        if (senderports.size() == 2)
        {
            data.append(nicknames.value(1).toStdString().c_str());
            data.append("|");
            data.append(colors.value(0).toStdString().c_str());
            data.append("|");
            data.append(turns.value(0).toStdString().c_str());
            data.append("|");
            socket.writeDatagram(data.data(), senders.value(0), senderports.value(0));
            data.clear();
            data.append(nicknames.value(0).toStdString().c_str());
            data.append("|");
            data.append(colors.value(1).toStdString().c_str());
            data.append("|");
            data.append(turns.value(1).toStdString().c_str());
            data.append("|");
            socket.writeDatagram(data.data(), senders.value(1), senderports.value(1));
            data.clear();
            nicknames.clear();
        }
    }
    else if (datastr == "leave" || datastr == "end")
    {
        if (senderport == senderports.value(0))
        {
            status = socket.writeDatagram(data.data(), senders.value(1), senderports.value(1));
            while (status == -1)
                status = socket.writeDatagram(data.data(), senders.value(1), senderports.value(1));
        }
        else if (senderport == senderports.value(1))
        {
            status = socket.writeDatagram(data.data(), senders.value(0), senderports.value(0));
            while (status == -1)
                status = socket.writeDatagram(data.data(), senders.value(0), senderports.value(0));
        }
        senders.clear();
        senderports.clear();
        nicknames.clear();
        data.clear();
        datastr.clear();
        qDebug() << "Игра окончена.";
    }
    else
    {
        if (senderport == senderports.value(0))
        {
            status = socket.writeDatagram(data.data(), senders.value(1), senderports.value(1));
            while (status == -1)
                status = socket.writeDatagram(data.data(), senders.value(1), senderports.value(1));
        }
        else if (senderport == senderports.value(1))
        {
            status = socket.writeDatagram(data.data(), senders.value(0), senderports.value(0));
            while (status == -1)
                status = socket.writeDatagram(data.data(), senders.value(0), senderports.value(0));
        }
    }
}

template <typename T>
bool Server::inArr(T &item, QVector<T> &arr)
{
    bool res = false;
    for (T i:arr)
    {
        if (i == item)
            res = true;
    }
    return res;
}

bool Server::generateRand()
{
    short res;
    bool rand = false;

    res = qrand() % 2; //По формуле qrand() % ((max + 1) - min) + min
    if (res == 0)
        rand = true;
    else
        rand = true;
    return rand;
}

void Server::setAddress()
{
    QList<QHostAddress> inf;
    inf = QHostInfo::fromName(QHostInfo::localHostName()).addresses();
    for (QHostAddress addr:inf)
    {
        if (addr.protocol() != QAbstractSocket::IPv6Protocol)
        {
            serveraddr = addr;
            QHostInfo::fromName(QHostInfo::localHostName());
        }
    }
}
