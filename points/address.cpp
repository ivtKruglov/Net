#include "address.h"
#include "ui_address.h"

Address::Address(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Address)
{
    ui->setupUi(this);

    correctIP =
    {
        {0, "(?:\\d+).(?:\\d+).(?:\\d+).(?:\\d+)::(?:\\d+)"}
    };
    gamewin = new Game();
    connect(gamewin, SIGNAL(mainwin()), this, SLOT(openmain()));
}

Address::~Address()
{
    delete ui;
    delete gamewin;
}

void Address::on_pushButton_clicked()
{
    QRegExp rx(correctIP.values(0).last());
    if (rx.indexIn(ui->lineEdit->text()) == -1 || ui->lineEdit_2->text() == "")
        ui->label_3->setText("Неккоректный адрес сервера или не задан ник клиента");
    else
    {
        QString text = ui->lineEdit->text();
        QString ip;
        QString port;
        int sep = text.indexOf(':');
        for (int i = 0; i < sep; ++i)
        {
            ip.append(text[i]);
        }
        for (int i = sep + 2; i < text.size(); ++i)
        {
            port.append(text[i]);
        }
        gamewin->newServerAddress(ip);
        gamewin->newServerPort(quint16(port.toInt()));
        gamewin->newNickname(ui->lineEdit_2->text());
        gamewin->show();
        this->close();
    }
}


void Address::openmain()
{
    emit mainwin();
    gamewin->clearGame();
}

void Address::on_pushButton_2_clicked()
{
    emit mainwin();
    this->close();
}
