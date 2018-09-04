#include "board.h"

int randomize_me_a_card(int max)
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator

    std::uniform_int_distribution<> distr(0, max); // define the range
    return distr(eng);
}

Board::Board(Player *n, Player *e, Player *s, Player *w)
    : north(n), east(e), south(s), west(w)
{
    for (int i = two; i <= ace ; i++)
    {
        for (int j = blue; j <= black; j++)
        {
            Card c;
            c.value = static_cast<e_card>(i);
            c.color = static_cast<e_color>(j);
            c.state = unused;
            if (five == i)
                c.points = 5;
            else if (ten == i || king == i)
                c.points = 10;
            else
                c.points = 0;
            ingame.push_back(c);
        };
    }
    for (int k = mahjong; k <= dragon; k++)
    {
        Card c;
        c.value = static_cast<e_card>(k);
        c.color = special;
        c.state = unused;
        if (phoenix == k)
            c.points = -25;
        else if (dragon ==k)
            c.points = 25;
        else
            c.points = 0;
        ingame.push_back(c);
    };

    for (int l = 0; l < MAX_CARDS; l+=4)
    {
        int r = randomize_me_a_card(ingame.size() - 1);
        south->hand.push_back(ingame[r]);
        ingame.erase(ingame.begin()+r);
        r = randomize_me_a_card(ingame.size() - 1);
        east->hand.push_back(ingame[r]);
        ingame.erase(ingame.begin()+r);
        r = randomize_me_a_card(ingame.size() - 1);
        north->hand.push_back(ingame[r]);
        ingame.erase(ingame.begin()+r);
        r = randomize_me_a_card(ingame.size() - 1);
        west->hand.push_back(ingame[r]);
        ingame.erase(ingame.begin()+r);
    }
}
