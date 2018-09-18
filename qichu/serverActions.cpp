#include "server.h"
#include "ui_server.h"

// methodes here are server actions. this are reaction after a client command have treated

void Server::updateHandshake()
{
    QJsonObject handShake;
    handShake.insert(JSON_command, JSON_handshake);

    if (this->players.size() >= 1)
        handShake.insert(JSON_p_north, this->playerNorth->getName());
    if (this->players.size() >= 2)
        handShake.insert(JSON_p_east, this->playerEast->getName());
    if (this->players.size() >= 3)
        handShake.insert(JSON_p_south, this->playerSouth->getName());
    if (this->players.size() >= 4)
        handShake.insert(JSON_p_west, this->playerWest->getName());
    this->broadCast(handShake);
}

QJsonObject Server::chatUpdate(QJsonObject chat)
{
    this->broadCast(chat);
    return (chat);
}

QJsonObject Server::gameStart(QJsonObject o)
{

    QJsonObject n = this->board->playerBoardStatus(this->playerNorth);
    QJsonObject e = this->board->playerBoardStatus(this->playerEast);
    QJsonObject s = this->board->playerBoardStatus(this->playerSouth);
    QJsonObject w = this->board->playerBoardStatus(this->playerWest);

    QJsonDocument output;

    n.insert(JSON_command, JSON_game_start);
    output.setObject(n);
    this->playerNorth->getSocket()->write(output.toJson());

    e.insert(JSON_command, JSON_game_start);
    output.setObject(e);
    this->playerEast->getSocket()->write(output.toJson());

    s.insert(JSON_command, JSON_game_start);
    output.setObject(s);
    this->playerSouth->getSocket()->write(output.toJson());

    w.insert(JSON_command, JSON_game_start);
    output.setObject(w);
    this->playerWest->getSocket()->write(output.toJson());

    qDebug()
            //<< "Discard: " << b->discard.size()     << "\n"
            << "South: "   << n << "\n"
            << "East: "    << e << "\n"
            << "North: "   << s << "\n"
            << "West: "    << w << "\n"
            << "blind board status" << this->board->blindBoardStatus() << "\n"
               ;

    if (this->board->ingame.size() == 0)
    {// all cards are dealt, game can begin
        // search first player to play
        for (Card c : this->playerNorth->hand)
            if (c.value == e_card::mahjong)
                this->board->turn = e_turn::north;
        for (Card c : this->playerEast->hand)
            if (c.value == e_card::mahjong)
                this->board->turn = e_turn::east;
        for (Card c : this->playerSouth->hand)
            if (c.value == e_card::mahjong)
                this->board->turn = e_turn::south;
        for (Card c : this->playerWest->hand)
            if (c.value == e_card::mahjong)
                this->board->turn = e_turn::west;
        this->playerTurn(this->board->turn);
    }


    return (o);
}

QJsonObject Server::playerTurn(e_turn turn)
{
    QJsonObject playerTurn;

    playerTurn.insert(JSON_command, JSON_end_turn);
    playerTurn.insert(JSON_player_turn, this->board->playerPosition[turn]->playerPositionName);
    playerTurn.insert(JSON_player, this->board->playerPosition[turn]->getName());

    this->broadCast(playerTurn);
    return (playerTurn);
}

QJsonObject Server::announced(QJsonObject announce)
{
    if (announce.contains(JSON_error))
        return (announce);

    QString player = announce.value(JSON_player).toString();
    QString req = announce.value(JSON_announce).toString();
    Player* p = this->playerSockets[this->players[player]];
    e_announce a;
    bool isValide = false;


    announce.insert(this->playerNorth->getName(),
                    (this->playerNorth->announceName.isEmpty())?(JSON_unknown):(this->playerNorth->announceName));
    announce.insert(this->playerEast->getName(),
                    (this->playerEast->announceName.isEmpty())?(JSON_unknown):(this->playerEast->announceName));
    announce.insert(this->playerSouth->getName(),
                    (this->playerSouth->announceName.isEmpty())?(JSON_unknown):(this->playerSouth->announceName));
    announce.insert(this->playerWest->getName(),
                    (this->playerWest->announceName.isEmpty())?(JSON_unknown):(this->playerWest->announceName));

    if (this->board->ingame.size() == (FINISH_DEAL*NB_PLAYER))
    { // waiting dealing last cards
        if (this->playerNorth->announceName.isEmpty() ||
                this->playerEast->announceName.isEmpty()  ||
                this->playerSouth->announceName.isEmpty() ||
                this->playerWest->announceName.isEmpty())
        { // waiting for another player announces
        }
        else
        { // every player want its cards
            // deal 6 cards
            this->board->dealCards(this->playerSockets.values(), FINISH_DEAL);

            // start game for test... but missing exchange parts
            this->gameStart(this->board->blindBoardStatus());
        }
    }
    else if (this->board->ingame.size() == ((FINISH_DEAL+START_DEAL)*NB_PLAYER))
    { // nothing is deal

    }
    else
    { // game is started

    }





    this->broadCast(announce);
    return (announce);
}

QJsonObject Server::exchanged(QJsonObject o)
{

    this->broadCast(o);
    return (o);
}

QJsonObject Server::cardPlayed(QJsonObject o)
{
    this->broadCast(o);
    this->turnFinished();
    return (o);
}

QJsonObject Server::checked(QJsonObject check)
{
    bool isPlayerTurn = false;
    // isPlayerTurn = Core::checkPlayerChecked(check, player)
    this->broadCast(check);
    return (check);
}

QJsonObject Server::turnFinished()
{
    QJsonObject playerTurn;

    playerTurn.insert(JSON_command, JSON_end_turn);
    playerTurn.insert(JSON_player_turn, this->board->currentPlayer()->playerPositionName);
    playerTurn.insert(JSON_player, this->board->currentPlayer()->getName());

    this->broadCast(playerTurn);

    if (!this->board->isGameOver())
    {
        this->playerTurn(this->board->turn);
        if (this->board->last_hand[0].value == dog)
            this->board->nextTurn();

        while (currentPlayer()->hand.size() == 0)
            this->board->nextTurn();
        this->playerTurn(this->board->turn);
    }
    else
        this->endGame(playerTurn);

    return (playerTurn);
}

QJsonObject Server::endGame(QJsonObject o)
{
    int verticalTeamScore;
    int horizontalTeamScore;
    if (this->board->ingameTeamNumber() == 1)
    {
        verticalTeamScore = (this->playerNorth->hand.size())?(0):(200);
        horizontalTeamScore = (this->playerWest->hand.size())?(0):(200);
    }
    else
    {
        Player *p = this->playerNorth;

        for (int loop = 0; loop < NB_PLAYER; loop++)
        {
            if ()
                ;
            p = p->pLeft;
        }
    }
    this->broadCast(o);
    return (o);
}

QJsonObject Server::sendError(QJsonObject o)
{
    this->broadCast(o);
    return (o);
}

