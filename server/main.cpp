#include <QCoreApplication>
#include "server.h"

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "Russian");
    QCoreApplication a(argc, argv);
    Server server;
    server.startServer();
    return a.exec();
}
