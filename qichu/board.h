#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <random>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "player.h"
#include "core.h"
#include "protocol.h"


class Board
{
public:
    Board(Player *n, Player *e, Player *s, Player *w);
    Player *north;
    Player *east;
    Player *south;
    Player *west;


    e_turn turn;
    void nextTurn() {this->turn = (e_turn)(((int)(this->turn) + 1) % 4);}
    int dealCards(QList<Player*> players, int nbCard);

    std::vector<Card> ingame;
    std::vector<Card> last_hand;
    std::vector<Card> discard;

    // ////////////////////////////////////////////
    // board members
    // ////////////////////////////////////////////
    QJsonObject blindBoardStatus();
    QJsonObject playerBoardStatus(Player* p);
    QJsonArray encodeCardList(std::vector<Card> cards);
    std::vector<Card> decodeCardList(QJsonArray cards);

};

#endif // BOARD_H
