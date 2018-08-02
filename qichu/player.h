#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>
#include <QTcpSocket>
#include "card.h"

namespace Ui {
class Player;
}

class Player : public QWidget
{
    Q_OBJECT

public:
    explicit Player(QWidget *parent, QString n, QTcpSocket *s);
    Player();
    ~Player();
    std::vector<Card> hand;
    std::vector<Card> upper_hand;
    std::vector<Card> won;
    Card left;
    Card right;
    Card front;

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
