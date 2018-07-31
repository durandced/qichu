#ifndef CARD_H
#define CARD_H

#include <vector>
#include <random>

#define ONE "Mahjong" // attrape-couillon
#define TWO "two"
#define THREE "three"
#define FOUR "four"
#define FIVE "five"
#define SIX "six"
#define SEVEN "seven" // WHAT'S IN THE BOX
#define EIGHT "eight"
#define NINE "nine"
#define TEN "ten"
#define JACK "jack" // She's got it
#define QUEEN "queen"
#define KING "king"
#define ACE "ace" // OF SPADES, THE ACE OF SPADES **air guitar**
#define MAHJONG "mahjong"
#define DOG "dog"
#define PHOENIX "phoenix"
#define DRAGON "dragon"

#define MAX_CARDS 56
enum e_card
{
    two,
    three,
    four,
    five,
    six,
    seven,
    eight,
    nine,
    ten,
    jack,
    queen,
    king,
    ace,
    mahjong,
    dog,
    phoenix,
    dragon
};

enum e_color
{
    blue,
    green,
    red,
    black,
    special
};

enum e_state
{
    unused,
    in_use,
    used
};

class Card
{
public:
    e_card  value;
    e_color color;
    e_state state;

};

class board
{
public:
    board();
    std::vector<Card> south;
    std::vector<Card> east;
    std::vector<Card> north;
    std::vector<Card> west;
    std::vector<Card> discard;
    std::vector<Card> ingame;
};

#endif // CARD_H
