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

    return (o);
}

QJsonObject Server::playerTurn(QJsonObject o)
{
    this->broadCast(o);
    return (o);
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

    if (req == JSON_none)
    {
        a = e_announce::no;
        if (p->announceName.isEmpty() || (p->announceName == JSON_none))
        {
            p->announceName = req;
            p->announce = a;
            isValide = true;
        }
        else
        {
            isValide = false;
        }

    }
    else if (req == JSON_tichu)
    {
        a = e_announce::tichu;
    }
    else if (req == JSON_grand_tichu)
    {
        a = e_announce::grandTichu;
    }
    else if (req == JSON_artichette)
    {
        a = e_announce::artichette;
    }

    if (!isValide)
    {
        announce.insert(JSON_error, JSON_announce_error);
        this->broadCast(announce);
        return (announce);
    }
    else
    {
        announce.insert(this->playerNorth->getName(),
                        (this->playerNorth->announceName.isEmpty())?(JSON_unknown):(this->playerNorth->announceName));
        announce.insert(this->playerEast->getName(),
                        (this->playerEast->announceName.isEmpty())?(JSON_unknown):(this->playerEast->announceName));
        announce.insert(this->playerSouth->getName(),
                        (this->playerSouth->announceName.isEmpty())?(JSON_unknown):(this->playerSouth->announceName));
        announce.insert(this->playerWest->getName(),
                        (this->playerWest->announceName.isEmpty())?(JSON_unknown):(this->playerWest->announceName));
        this->broadCast(announce);
    }

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
            this->gameStart(this->board->blindBoardStatus());
        }
    }
    else if (this->board->ingame.size() == ((FINISH_DEAL+START_DEAL)*NB_PLAYER))
    { // nothing is deal

    }
    else
    { // game is started

    }





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
    return (o);
}

QJsonObject Server::checked(QJsonObject check)
{
    bool isPlayerTurn = false;
    // isPlayerTurn = Core::checkPlayerChecked(check, player)
    this->broadCast(check);
    return (check);
}

QJsonObject Server::turnFinished(QJsonObject o)
{
    this->broadCast(o);
    return (o);
}

QJsonObject Server::endGame(QJsonObject o)
{
    this->broadCast(o);
    return (o);
}

QJsonObject Server::sendError(QJsonObject o)
{
    this->broadCast(o);
    return (o);
}

