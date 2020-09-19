#ifndef SERVER_H
#define SERVER_H

#include <QString>
#include <QUdpSocket>

class Server:public QUdpSocket
{
    Q_OBJECT
public:
    Server();
    ~Server();
    QUdpSocket *socket;                                  // Сокет UDP
    QByteArray *data;                                    // Данные, передаваемые по сокету
    QHostAddress *sender;                                // Адрес клиента
    QVector<quint16> *senderports;                       // Порты клиентов, отправляющих данные
    quint16 *senderport;                                 // Порт клиента, от которого пришла датаграмма
    bool inArr(quint16 *port, QVector<quint16> *arr);    // Проверка на вхождение элемнта в массив
    void setTurn();                                      // Определение первого хода

public slots:
    void startServer();                                  // Старт сервера
    void socketReady();                                  // Действия при поступлении дейтаграммы на сервер
};

#endif // SERVER_H
