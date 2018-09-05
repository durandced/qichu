#ifndef CORE_H
#define CORE_H

#include <vector>
#include "card.h"

enum e_turn {
    north = 0,
    west,
    south,
    east
};

enum e_announce {
    unknown = 0,
    no,
    tichu,
    grandTichu,
    artichette,
};

enum e_combi {
    none = 0,
    card,           // Case 1
    pair,           // check_svector_spairs
    two_pairs,      // check_svector_spairs
    three_pairs,    // check_svector_spairs
    four_pairs,     // check_svector_spairs
    five_pairs,     // check_svector_spairs
    six_pairs,      // check_svector_spairs
    seven_pairs,    // check_svector_spairs
    triple,         // check_svector_triple
    full_house,
    straight_five,
    straight_six,
    straight_seven,
    straight_eight,
    straight_nine,
    straight_ten,
    straight_eleven,
    straight_twelve,
    straight_thirteen,
    straight_fourteen,
    four_of_a_kind, // check_svector_foak
    straight_flush_five,
    straight_flush_six,
    straight_flush_seven,
    straight_flush_eight,
    straight_flush_nine,
    straight_flush_ten,
    straight_flush_eleven,
    straight_flush_twelve,
    straight_flush_thirteen,
    straight_flush_fourteen
};

struct s_combi {
    e_combi type;
    int value;
};

s_combi is_vector_combi(std::vector<Card> c);
bool verify_and_deal_exchange();

#endif // CORE_H
