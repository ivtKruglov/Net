#include "server.h"

Server::Server(){}


Server::~Server(){}


void Server::startServer()
{
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress::LocalHost, 1000);
    senderports = new QVector<quint16>();
    connect(socket, SIGNAL(readyRead()), this, SLOT(socketReady()));
    qDebug()<<"Сервер запущен";
}


void Server::socketReady()
{
    data = new QByteArray();
    sender = new QHostAddress();
    senderport = new quint16();

    data->resize(quint16(socket->pendingDatagramSize()));
    socket->readDatagram(data->data(), data->size(), sender, senderport);
    if (senderports->size() < 2 && not inArr(senderport, senderports))
        senderports->push_back(*senderport);
    else
    {
        if (*senderport == senderports->value(0))
            socket->writeDatagram(data->data(), *sender, senderports->value(1));
        else if (*senderport == senderports->value(1))
            socket->writeDatagram(data->data(), *sender, senderports->value(0));
    }

    qDebug()<<"Данные получены: "<<data->data();
    qDebug()<<"Адрес отправителя: "<<sender->toString();
    qDebug()<<"Порт отправителя: "<<quint16(*senderport);
}


bool Server::inArr(quint16 *port, QVector<quint16> *arr)
{
    bool res = false;
    for (quint16 p:*arr)
    {
        if (p == *port)
            res = true;
    }
    return res;
}


void Server::setTurn()
{
    short *rand = new short();
    bool *pl_1 = new bool();
    bool *pl_2 = new bool();

    *pl_1 = false;
    *pl_2 = false;
    *rand = qrand() % ((1 + 1) - 0) + 0; //По формуле qrand() % ((max + 1) - min) + min
    if (*rand == 0)
        *pl_1 = true;
    else
        *pl_2 = true;
}
