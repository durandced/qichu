#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <map>
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
    Board(Player *n, Player *e, Player *s, Player *w, int max = 1000);
    Board(Player *cli);
    ~Board();
public:
    Player *north;
    Player *east;
    Player *south;
    Player *west;
    int maxScore;

    Player *client;

    Player *firstFinish;
    Player *lastFinish;
    Player *leader;

    int verticalTeamScore;
    int horizontalTeamScore;
    int verticalTeamPoint;
    int horizontalTeamPoint;
    std::map<e_turn, Player *> playerPosition;

    e_turn turn;
    int dealCards(QList<Player*> players, int nbCard);
    void setExchange();

    void nextTurn() {this->turn = (e_turn)(((int)(this->turn) + 1) % 4);}
    int ingamePlayerNumber();
    int ingameTeamNumber();

    bool isGameOver();
    bool isBoardOver();
    bool countScore();
    void resetGame();
    void resetBoard();

    Player *currentPlayer() { return this->playerPosition[this->turn]; }

    std::vector<Card> ingame;
    std::vector<Card> last_hand;
    std::vector<Card> discard;

    // ////////////////////////////////////////////
    // board members
    // ////////////////////////////////////////////
    QJsonObject blindBoardStatus();
    QJsonObject playerBoardStatus(Player* p);
    static QJsonArray encodeCardList(std::vector<Card> cards);
    static std::vector<Card> decodeCardList(QJsonArray cards);

};

#endif // BOARD_H
