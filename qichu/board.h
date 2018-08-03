#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <random>
#include "player.h"
#include "core.h"

class Board
{
public:
    Board(Player *n, Player *e, Player *s, Player *w);
    Player *north;
    Player *east;
    Player *south;
    Player *west;


    std::vector<Card> ingame;
    std::vector<Card> last_hand;
};

#endif // BOARD_H
