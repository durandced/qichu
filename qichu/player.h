#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>
#include <QTcpSocket>

namespace Ui {
class Player;
}

class Player : public QWidget
{
    Q_OBJECT

public:
    explicit Player(QWidget *parent, QString n, QTcpSocket *s);
    ~Player();

public:
    QTcpSocket *getSocket() {return this->socket;}
    QString getName() {return this->name;}
private:
    QTcpSocket *socket;
    QString name;


private:
    Ui::Player *ui;
};

#endif // PLAYER_H
