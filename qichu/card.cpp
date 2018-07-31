#include "card.h"

int randomize_me_a_card(int max)
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 eng(rd()); // seed the generator

    std::uniform_int_distribution<> distr(0, max); // define the range
    return distr(eng);
}

board::board()
{
    for (int i = two; i <= ace ; i++)
    {
        for (int j = blue; j <= black; j++)
        {
            Card C;
            C.value = static_cast<e_card>(i);
            C.color = static_cast<e_color>(j);
            C.state = unused;
            discard.push_back(C);
        };
    }
    for (int k = mahjong; k <= dragon; k++)
    {
        Card C;
        C.value = static_cast<e_card>(k);
        C.color = special;
        C.state = unused;
        discard.push_back(C);
    };

    for (int l = 0; l < MAX_CARDS; l+=4)
    {
        int r = randomize_me_a_card(discard.size() - 1);
        south.push_back(discard[r]);
        discard.erase(discard.begin()+r);
        r = randomize_me_a_card(discard.size() - 1);
        east.push_back(discard[r]);
        discard.erase(discard.begin()+r);
        r = randomize_me_a_card(discard.size() - 1);
        north.push_back(discard[r]);
        discard.erase(discard.begin()+r);
        r = randomize_me_a_card(discard.size() - 1);
        west.push_back(discard[r]);
        discard.erase(discard.begin()+r);
    }
}
