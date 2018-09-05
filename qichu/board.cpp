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
    n->playerPosition = e_turn::north;
    e->playerPosition = e_turn::east;
    s->playerPosition = e_turn::south;
    w->playerPosition = e_turn::west;

    n->pLeft = e;
    e->pLeft = s;
    s->pLeft = w;
    w->pLeft = n;

    n->pRight = w;
    e->pRight = n;
    s->pRight = e;
    w->pRight = s;

    n->pFront = s;
    e->pFront = w;
    s->pFront = n;
    w->pFront = e;

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
}

int Board::dealCards(QList<Player*> players, int nbCard)
{
    int r;
    int deal = 0;
    for (int l = 0; l < nbCard; l++)
    {
        foreach (Player* p, players)
        {
            if (!this->ingame.size())
                return deal;
            r = randomize_me_a_card(this->ingame.size() - 1);
            p->hand.push_back(this->ingame[r]);
            this->ingame.erase(this->ingame.begin()+r);
        }
    }
    return deal;
}


QJsonArray Board::encodeCardList(std::vector<Card> cards)
{
    QJsonArray jsonCards;
    QString code;

    for (auto card : cards)
    {
        code = QString("%1%2%3")
                .arg(CARD_CODER(card.value))
                .arg(COLOR_CODER(card.color))
                .arg(STATE_CODER(card.state))
                ;
        jsonCards.append(code);
    }
    return jsonCards;
}

std::vector<Card> Board::decodeCardList(QJsonArray cards)
{
    std::vector<Card> vectorCards;
    Card code;

    foreach (QJsonValue card, cards)
    {
        code.value = CARD_DECODER(card.toString().at(0).toLatin1());
        code.color = COLOR_DECODER(card.toString().at(1).toLatin1());
        code.state = STATE_DECODER(card.toString().at(2).toLatin1());
        vectorCards.push_back(code);
    }
    return vectorCards;
}

QJsonObject Board::playerBoardStatus(Player* p)
{
    QJsonObject blind = this->blindBoardStatus();

    QJsonObject hand = blind.value(p->getName()).toObject();

    hand.insert(JSON_hand, this->encodeCardList(p->hand));
    hand.insert(JSON_won, this->encodeCardList(p->won));
    blind.insert(p->getName(), hand);

    return blind;
}


QJsonObject Board::blindBoardStatus()
{
    QJsonObject blindBoardState;
    QJsonObject boardState;
    QJsonObject pNorth;
    QJsonObject pEast ;
    QJsonObject pSouth;
    QJsonObject pWest ;

    blindBoardState.insert(JSON_discard, this->encodeCardList(this->discard));
    blindBoardState.insert(JSON_deal_size, (int)this->ingame.size());
    blindBoardState.insert(JSON_last_played, this->encodeCardList(this->last_hand));

    pNorth.insert(JSON_hand, (int)this->north->hand.size());
    pNorth.insert(JSON_won , (int)this->north->won.size());
    pNorth.insert(JSON_lead, this->north->lead);

    pEast.insert(JSON_hand, (int)this->east->hand.size());
    pEast.insert(JSON_won , (int)this->east->won.size());
    pEast.insert(JSON_lead, this->east->lead);

    pSouth.insert(JSON_hand, (int)this->south->hand.size());
    pSouth.insert(JSON_won , (int)this->south->won.size());
    pSouth.insert(JSON_lead, this->south->lead);

    pWest.insert(JSON_hand, (int)this->west->hand.size());
    pWest.insert(JSON_won , (int)this->west->won.size());
    pWest.insert(JSON_lead, this->west->lead);

    blindBoardState.insert(JSON_boars_status, boardState);
    blindBoardState.insert(this->north->getName(), pNorth);
    blindBoardState.insert(this->east->getName(), pEast );
    blindBoardState.insert(this->south->getName(), pSouth);
    blindBoardState.insert(this->west->getName(), pWest );

    return blindBoardState;
}
