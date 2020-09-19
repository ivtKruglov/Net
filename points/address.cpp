#include "address.h"
#include "ui_address.h"

Address::Address(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Address)
{
    ui->setupUi(this);

    gamewin = new Game();
    connect(gamewin, SIGNAL(mainwin()), this, SLOT(openmain()));
}

Address::~Address()
{
    delete ui;
}

void Address::on_pushButton_clicked()
{
    if (ui->lineEdit->text() != "")
    {
        gamewin->address->setAddress(ui->lineEdit->text());
        gamewin->show();
        gamewin->socket->writeDatagram(gamewin->data->data(), QHostAddress::LocalHost, 1000);
        gamewin->data->clear();
        this->close();
    }
    else
        ui->lineEdit->setText("IP клиента не задан!");
}


void Address::openmain()
{
    emit mainwin();
}
