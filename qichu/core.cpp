#include "core.h"
#include <algorithm>

bool has_vector_phoenix(std::vector<Card> c)
{
    for (auto &v: c)
        if (v.value == phoenix)
            return true;
    return false;
}

/* Takes two cards as parameter.
 * Returns true if it's a pair, false otherwise.
*/
bool check_pair(Card c1, Card c2)
{
    return (c1.value == c2.value);
}

s_combi check_svector_straight(std::vector<Card> c)
{
    s_combi ret;
    ret.type = none;
    ret.value = 0;
    int errors = 0;
    int px = 0;
    int i = 0;
    e_color color = c[0].color;
    int flush = e_combi(straight_flush_five - straight_five);

    if (has_vector_phoenix(c))
    {
        px = 1;
    }

    for (auto &v : c)
        if(v.color != color)
            if (v.value != phoenix)
                flush = 0;

    for (i = 0; i < c.size() - 1 - px; i++)
    {
        errors += (c[i].value - c[i + 1].value + 1);
        ret.value = c[i + 1].value;
        if (i >= (3 - px))
        {
            ret.type = (e_combi)(straight_five + (i - 3 + px) + flush) ;
            if (c.back().p_value > ret.value)
                ret.value = c.back().p_value;
        }
    }
    if (errors)
    {
        if (errors + px)
        {
            ret.value = 0;
            ret.type = none;
        }
    }
    return ret;

}



s_combi check_svector_spairs(std::vector<Card> c)
{
    s_combi ret;
    ret.type = none;
    ret.value = 0;
    std::vector<Card> tmp = c;

    if (has_vector_phoenix(tmp))
    {
        for (int i = 0; i < tmp.size() - 1; i++)
        {
            if (!check_pair(tmp[i], tmp[i + 1]))
                tmp.back().value = tmp[i].value;
            else
                i++;
        }
    }
    std::sort(tmp.begin(), tmp.end());

    for (int i = 0; i < tmp.size() - 1; i++)
    {
        if (check_pair(tmp[i], tmp[i + 1]))
        {
            ret.value = tmp[i].value;
            ret.type = (e_combi)(pair + (i / 2));
            i++;
            continue;
        }
        else
        {
            ret.type = none;
            ret.value = 0;
            return ret;
        }
    }
    return ret;
}

/* Takes a sorted card vector as parameter.
 * Returns 0 if not a four of a kind.
 * Returns the value of the foak otherwise */
int check_svector_foak(std::vector<Card> c)
{
    if (c.size() != 4)
        return none;
    if (has_vector_phoenix(c))
        if (c[0].value != c[1].value || c[0].value != c[2].value)
            return none;
        else
            return c[0].value;
    if (c[0].value != c[1].value || c[0].value != c[2].value || c[0].value != c[3].value)
        return none;
    return c[0].value;
}

/* Takes a sorted card vector as parameter.
 * Returns 0 if not a triple.
 * Returns the value of the triple otherwise */
int check_svector_triple(std::vector<Card> c)
{
    if (c.size() != 3)
        return none;
    if (has_vector_phoenix(c))
        if(c[0].value != c[1].value)
            return none;
        else
            return c[0].value;
    if (c[0].value != c[1].value || c[0].value != c[2].value)
        return none;
    return c[0].value;
}

s_combi check_svector_fullhouse(std::vector<Card> c)
{
    s_combi ret;
    ret.type = none;
    ret.value = 0;
    std::vector<Card> tmp = c;
    std::vector<Card> first;
    std::vector<Card> second;

    if (has_vector_phoenix(tmp))
        tmp.back().value = (e_card)tmp.back().p_value;

    std::sort(tmp.begin(), tmp.end());
    first.push_back(tmp[0]);
    first.push_back(tmp[1]);
    first.push_back(tmp[2]);
    if (!check_svector_triple(first))
    {
        if (check_pair(tmp[0], tmp[1]))
        {
            first.pop_back();
            second.push_back(tmp[2]);
            second.push_back(tmp[3]);
            second.push_back(tmp[4]);
            if (!check_svector_triple(second))
                return ret;
            else
            {
                ret.type = full_house;
                ret.value = (tmp[3].value*100 + tmp[0].value);
            }
        }
    }
    else
    {
        if (check_pair(tmp[3], tmp[4]))
        {
            ret.type = full_house;
            ret.value = (tmp[0].value * 100 + tmp [3].value);
        }
    }
    return ret;
}

s_combi is_vector_combi(std::vector<Card> c)
{
    s_combi res;
    res.type = none;
    res.value = 0;

    std::sort(c.begin(), c.end());

    switch (c.size())
    {
    case 1:
        res.type = card;
        if (c[0].value == phoenix)
            res.value = c[0].p_value;
        else
            res.value = c[0].value;
        break;
    case 2:
        res = check_svector_spairs(c);
        break;
    case 3:
        res.value = check_svector_triple(c);
        if (!res.value)
            res.type = triple;
        break;
    case 4:
        res.value = check_svector_foak(c);
        if (!res.value)
            res.type = four_of_a_kind;
        else
            res = check_svector_spairs(c);
        break;
    case 5:
        res = check_svector_straight(c);
        if (!res.value)
           res = check_svector_fullhouse(c);
        break;
    case 6:
        res = check_svector_spairs(c);
        if (!res.value)
            res = check_svector_straight(c);
        break;
    case 7:
        res = check_svector_straight(c);
        break;
    case 8:
        res = check_svector_spairs(c);
        if (!res.value)
            res = check_svector_straight(c);
        break;
    case 9:
        res = check_svector_straight(c);
        break;
    case 10:
        res = check_svector_spairs(c);
        if (!res.value)
            res = check_svector_straight(c);
        break;
    case 11:
        res = check_svector_straight(c);
        break;
    case 12:
        res = check_svector_spairs(c);
        if (!res.value)
            res = check_svector_straight(c);
        break;
    case 13:
        res = check_svector_straight(c);
        break;
    case 14:
        res = check_svector_spairs(c);
        if(!res.value)
            res = check_svector_straight(c);
        break;
    }
    return res;
}
