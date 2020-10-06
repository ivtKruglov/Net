#ifndef SERVER_H
#define SERVER_H

#include <QString>
#include <QUdpSocket>
#include <QTcpSocket>

class Server:public QUdpSocket
{
    Q_OBJECT
public:
    Server();
    ~Server();
    template <typename T>
    bool inArr(T &item, QVector<T> &arr); // Проверка на вхождение элемнта в массив
    bool generateRand();                  // Рандом
    void setAddress();                    // Узнать адрес данного сервера и запомнить, чтобы связать с портом этого же клиента через сокет

public slots:
    void startServer();                                  // Старт сервера
    void socketReady();                                  // Действия при поступлении дейтаграммы на сервер

private:
    QUdpSocket socket;                                   // Сокет UDP
    QByteArray data;                                     // Данные, передаваемые по сокету
    QHostAddress sender;                                 // Адрес клиента
    QVector<QString> nicknames;                          // Ники клиентов
    QVector<quint16> senderports;                        // Порты клиентов
    QVector<QHostAddress> senders;                       // IP-адреса клиентов
    quint16 senderport;                                  // Порт клиента, от которого пришла датаграмма
    qint64 status;                                       // Результат отправки или чтения пакета
    QHostAddress serveraddr;
};

#endif // SERVER_H
