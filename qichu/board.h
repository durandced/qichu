#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <random>
#include "player.h"

class Board
{
public:
    Board();
    Player north;
    Player east;
    Player south;
    Player west;

    std::vector<Card> discard;
    std::vector<Card> ingame;
    std::vector<Card> last_hand;
};

#endif // BOARD_H
