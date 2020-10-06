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

private slots:
    void on_pushButton_clicked();      // Начать игру
    void on_pushButton_2_clicked();    // Отмена
    void openmain();                   // Отмена

private:
    Ui::Address *ui;
    Game *gamewin;
    QMap<int, QString> correctIP;
};

#endif // ADDRESS_H
