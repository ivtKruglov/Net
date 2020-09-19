#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    address = new Address();
    connect(address, SIGNAL(mainwin()), this, SLOT(show()));
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    address->show();
    this->close();
}


void MainWindow::on_pushButton_2_clicked()
{
    this->close();
}
