#include "mainwindow.h"
#include "board.h"
#include "core.h"
#include <QApplication>
#include <algorithm>


void pretty_print_card(Card c)
{
    std::vector<QString> cname = {TWO, THREE, "four", "five",
                                        "six", "seven", "eight", "nine", "ten",
                                        "jack", "queen", "king", "ace",
                                        ONE, "dog", "phoenix", "dragon"};
    std::vector<QString> ccolor = {"blue", "green", "red", "black", "special"};
    std::vector<QString> cstate = {"unused", "in_use", "used"};
    qDebug() << cname[c.value] << ccolor[c.color] << cstate[c.state] << c.points;

}

void print_vector(std::vector<Card> v)
{
    for (auto &c : v)
        pretty_print_card(c);
}

std::vector<Card> generate_cards()
{
    std::vector<Card> ret;
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
            ret.push_back(c);
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
        ret.push_back(c);
    };
    return ret;

}

s_combi test_legit_pair()
{
    s_combi ret;
    std::vector<Card> h = generate_cards();
    std::vector<Card> pairs;
    pairs.push_back(h[0]);
    pairs.push_back(h[1]);
    print_vector(pairs);
    ret = is_vector_combi(pairs);
    qDebug() << "Ret:" << ret.type << ret.value;
    return ret;
}

s_combi test_legit_pair2()
{
    s_combi ret;
    std::vector<Card> h = generate_cards();
    std::vector<Card> pairs;
    pairs.push_back(h[0]);
    pairs.push_back(h[1]);
    pairs.push_back(h[4]);
    pairs.push_back(h[5]);
    print_vector(pairs);
    ret = is_vector_combi(pairs);
    qDebug() << "Ret:" << ret.type << ret.value;
    return ret;
}

s_combi test_legit_pair3()
{
    s_combi ret;
    std::vector<Card> h = generate_cards();
    std::vector<Card> pairs;
    pairs.push_back(h[0]);
    pairs.push_back(h[1]);
    pairs.push_back(h[4]);
    pairs.push_back(h[5]);
    pairs.push_back(h[8]);
    pairs.push_back(h[9]);
    print_vector(pairs);
    ret = is_vector_combi(pairs);
    qDebug() << "Ret:" << ret.type << ret.value;
    return ret;
}

s_combi test_legit_pair4()
{
    s_combi ret;
    std::vector<Card> h = generate_cards();
    std::vector<Card> pairs;
    pairs.push_back(h[0]);
    pairs.push_back(h[1]);
    pairs.push_back(h[4]);
    pairs.push_back(h[5]);
    pairs.push_back(h[8]);
    pairs.push_back(h[9]);
    pairs.push_back(h[12]);
    pairs.push_back(h[13]);
    print_vector(pairs);
    ret = is_vector_combi(pairs);
    qDebug() << "Ret:" << ret.type << ret.value;
    return ret;
}

s_combi test_legit_pair5()
{
    s_combi ret;
    std::vector<Card> h = generate_cards();
    std::vector<Card> pairs;
    pairs.push_back(h[0]);
    pairs.push_back(h[1]);
    pairs.push_back(h[4]);
    pairs.push_back(h[5]);
    pairs.push_back(h[8]);
    pairs.push_back(h[9]);
    pairs.push_back(h[12]);
    pairs.push_back(h[13]);
    pairs.push_back(h[16]);
    pairs.push_back(h[17]);
    print_vector(pairs);
    ret = is_vector_combi(pairs);
    qDebug() << "Ret:" << ret.type << ret.value;
    return ret;
}

s_combi test_legit_pair6()
{
    s_combi ret;
    std::vector<Card> h = generate_cards();
    std::vector<Card> pairs;
    pairs.push_back(h[0]);
    pairs.push_back(h[1]);
    pairs.push_back(h[4]);
    pairs.push_back(h[5]);
    pairs.push_back(h[8]);
    pairs.push_back(h[9]);
    pairs.push_back(h[12]);
    pairs.push_back(h[13]);
    pairs.push_back(h[16]);
    pairs.push_back(h[17]);
    pairs.push_back(h[20]);
    pairs.push_back(h[21]);
    print_vector(pairs);
    ret = is_vector_combi(pairs);
    qDebug() << "Ret:" << ret.type << ret.value;
    return ret;
}

s_combi test_legit_pair7()
{
    s_combi ret;
    std::vector<Card> h = generate_cards();
    std::vector<Card> pairs;
    pairs.push_back(h[0]);
    pairs.push_back(h[1]);
    pairs.push_back(h[4]);
    pairs.push_back(h[54]);
    pairs.push_back(h[8]);
    pairs.push_back(h[9]);
    pairs.push_back(h[12]);
    pairs.push_back(h[13]);
    pairs.push_back(h[16]);
    pairs.push_back(h[17]);
    pairs.push_back(h[20]);
    pairs.push_back(h[21]);
    pairs.push_back(h[24]);
    pairs.push_back(h[25]);
    print_vector(pairs);
    ret = is_vector_combi(pairs);
    qDebug() << "Ret:" << ret.type << ret.value;
    return ret;
}

s_combi test_fake_pair7()
{
        s_combi ret;
        std::vector<Card> h = generate_cards();
        std::vector<Card> pairs;
        pairs.push_back(h[0]);
        pairs.push_back(h[1]);
        pairs.push_back(h[4]);
        pairs.push_back(h[54]);
        pairs.push_back(h[8]);
        pairs.push_back(h[9]);
        pairs.push_back(h[12]);
        pairs.push_back(h[18]);
        pairs.push_back(h[16]);
        pairs.push_back(h[17]);
        pairs.push_back(h[20]);
        pairs.push_back(h[21]);
        pairs.push_back(h[24]);
        pairs.push_back(h[25]);
        print_vector(pairs);
        ret = is_vector_combi(pairs);
        qDebug() << "Ret:" << ret.type << ret.value;
        return ret;
}

s_combi test_straight5()
{
        s_combi ret;
        std::vector<Card> h = generate_cards();
        std::vector<Card> pairs;
        pairs.push_back(h[0]);
        pairs.push_back(h[4]);
        pairs.push_back(h[12]);
        pairs.push_back(h[16]);
        pairs.push_back(h[54]);
        print_vector(pairs);
        ret = is_vector_combi(pairs);
        qDebug() << "Ret:" << ret.type << ret.value;
        return ret;
}

s_combi test_fake_straight5()
{
        s_combi ret;
        std::vector<Card> h = generate_cards();
        std::vector<Card> pairs;
        pairs.push_back(h[0]);
        pairs.push_back(h[4]);
        pairs.push_back(h[8]);
        pairs.push_back(h[12]);
        pairs.push_back(h[42]);
        print_vector(pairs);
        ret = is_vector_combi(pairs);
        qDebug() << "Ret:" << ret.type << ret.value;
        return ret;
}

s_combi test_full_house()
{
        s_combi ret;
        std::vector<Card> h = generate_cards();
        std::vector<Card> pairs;
        pairs.push_back(h[0]);
        pairs.push_back(h[1]);
        pairs.push_back(h[12]);
        pairs.push_back(h[13]);
        pairs.push_back(h[14]);
        print_vector(pairs);
        ret = is_vector_combi(pairs);
        qDebug() << "Ret:" << ret.type << ret.value;
        return ret;
}

s_combi test_fake_house()
{
        s_combi ret;
        std::vector<Card> h = generate_cards();
        std::vector<Card> pairs;
        pairs.push_back(h[0]);
        pairs.push_back(h[1]);
        pairs.push_back(h[12]);
        pairs.push_back(h[14]);
        pairs.push_back(h[42]);
        print_vector(pairs);
        ret = is_vector_combi(pairs);
        qDebug() << "Ret:" << ret.type << ret.value;
        return ret;
}


s_combi test_fake_pair()
{
    s_combi ret;
    std::vector<Card> h = generate_cards();
    std::vector<Card> pair_two;
    pair_two.push_back(h[0]);
    pair_two.push_back(h[8]);
    print_vector(pair_two);
    ret = is_vector_combi(pair_two);
    qDebug() << "Ret:" << ret.type << ret.value;
    return ret;
}

s_combi test_legit_pair_phoenix()
{
    s_combi ret;
    std::vector<Card> h = generate_cards();
    std::vector<Card> pair_two;
    pair_two.push_back(h[0]);
    pair_two.push_back(h[54]);
    print_vector(pair_two);
    ret = is_vector_combi(pair_two);
    qDebug() << "Ret:" << ret.type << ret.value;
    return ret;
}

void test_combis()
{
    s_combi ret;
    ret.value = none;
    ret.type = none;

/*    ret = test_legit_pair();
    ret = test_fake_pair();
    ret = test_legit_pair_phoenix();
    ret = test_legit_pair2();
    ret = test_legit_pair3();
    ret = test_legit_pair4();
    ret = test_legit_pair5();
    ret = test_legit_pair6();
    ret = test_legit_pair7();
    ret = test_fake_pair7();
    ret = test_straight5();
    ret = test_fake_straight5(); */
    ret = test_full_house();
    ret = test_fake_house();
}

void test()
{
    Player *north = new Player();
    Player *east = new Player();
    Player *south = new Player();
    Player *west = new Player();

    Board *b = new Board(north, east, south, west);
    qDebug() << "Discard: " << b->ingame.size() << "\n"
             << "South: " << b->south->hand.size() << "\n"
             << "East: " << b->east->hand.size() << "\n    "
             << "North: " << b->north->hand.size() << "\n"
             << "West: " << b->west->hand.size() << "\n";
    std::sort(b->east->hand.begin(), b->east->hand.end());
    qDebug() << "Player East:";
    print_vector(b->east->hand);
    qDebug() << "Player West:";
    print_vector(b->west->hand);
    qDebug() << "Player South:";
    print_vector(b->south->hand);
    qDebug() << "Player North:";
    print_vector(b->north->hand);
    b->east->won = b->east->hand;
    b->west->won = b->west->hand;
    b->south->won = b->south->hand;
    b->north->won = b->north->hand;
    qDebug() << "points east: " << b->east->count_points();
    qDebug() << "points west: " << b->west->count_points();
    qDebug() << "points north: " << b->north->count_points();
    qDebug() << "points south: " << b->south->count_points();

}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    test_combis();
    return a.exec();
}
