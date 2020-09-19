#ifndef ADDRESS_H
#define ADDRESS_H

#include "game.h"
#include <QDialog>

namespace Ui {
class Address;
}

class Address : public QDialog
{
    Q_OBJECT

public:
    explicit Address(QWidget *parent = nullptr);
    ~Address();

signals:
    void mainwin();

public slots:
    void openmain();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Address *ui;
    Game *gamewin;
};

#endif // ADDRESS_H
