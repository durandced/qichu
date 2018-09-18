#ifndef CARD_H
#define CARD_H

#define ONE "Mahjong" // attrape-couillon
#define TWO "two" // for tea
#define THREE "three"
#define FOUR "four"
#define FIVE "five"
#define SIX "six"
#define SEVEN "seven" // WHAT'S IN THE BOX
#define EIGHT "eight"
#define NINE "nine" // inch nails
#define TEN "ten"
#define JACK "jack" // She's got it
#define QUEEN "queen" // play the game ! I want it all
#define KING "king"
#define ACE "ace" // OF SPADES, THE ACE OF SPADES **air guitar**
#define MAHJONG "mahjong"
#define DOG "dog" // who let it out! WHO- WHO- WHOWHO
#define PHOENIX "phoenix"
#define DRAGON "dragon" // imagine

#define MAX_CARDS 56
#define NB_PLAYER 4
#define START_DEAL 8
#define FINISH_DEAL 6
#define MAX_DEAL (START_DEAL+FINISH_DEAL)


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
    dog,  // chien
    phoenix,
    dragon
};

#define CARD_CODE "234567890jqkamcpd" // c chien
#define CARD_CODER(x) (QString(CARD_CODE).at((int)(x)))
#define CARD_DECODER(x) ((e_card)(QString(CARD_CODE).indexOf((char)(x))))

enum e_color
{
    blue,
    green,
    red,
    black, // noir
    special
};

#define COLOR_CODE "bgrns" // n noir
#define COLOR_CODER(x) (QString(COLOR_CODE).at((int)(x)))
#define COLOR_DECODER(x) ((e_color)(QString(COLOR_CODE).indexOf((char)(x))))

enum e_state
{
    unused, // not
    in_use,
    used
};

#define STATE_CODE "niu" // n not
#define STATE_CODER(x) (QString(STATE_CODE).at((int)(x)))
#define STATE_DECODER(x) ((e_state)(QString(STATE_CODE).indexOf((char)(x))))

class Card
{

public:
    bool operator<(const Card& other);
    e_card  value;
    e_color color;
    e_state state;
    int     points;
    int     p_value;
};

#endif // CARD_H
