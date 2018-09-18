#ifndef PLAYER_H
#define PLAYER_H

#include <QWidget>
#include <QTcpSocket>
#include "card.h"
#include "core.h"

namespace Ui {
class Player;
}

class Player : public QWidget
{
    Q_OBJECT

public:
    explicit Player(QWidget *parent, QString n, QTcpSocket *s);
    Player(); //FIXME: Remove that shit
    ~Player();
// Class Objects
    e_turn playerPosition;
    QString playerPositionName;
    Player* pLeft;
    Player* pRight;
    Player* pFront;
    std::vector<Card> hand;
    std::vector<Card> upper_hand;
    std::vector<Card> won;
    bool lead;
    Card left;
    Card right;
    Card front;
// Class Methods
    int count_points(); //count points in the won vector
// Qt Stuff
    QTcpSocket *getSocket() {return this->socket;}
    QString getName() {return this->name;}
    QString announceName;
    e_announce announce;
private:
    QTcpSocket *socket;
    QString name;


private:
    Ui::Player *ui;
};

#endif // PLAYER_H
