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

QJsonObject Server::appendAndSendCompleteBoardStatus(QJsonObject command)
{
    QJsonObject no = this->board->playerBoardStatus(this->playerNorth);
    QJsonObject ea = this->board->playerBoardStatus(this->playerEast);
    QJsonObject so = this->board->playerBoardStatus(this->playerSouth);
    QJsonObject we = this->board->playerBoardStatus(this->playerWest);


    foreach (QString key, command.keys())
    {
        no.insert(key, command.value(key));
        ea.insert(key, command.value(key));
        so.insert(key, command.value(key));
        we.insert(key, command.value(key));
    }

    QJsonDocument output;

    output.setObject(no);
    this->playerNorth->getSocket()->write(output.toJson());

    output.setObject(ea);
    this->playerEast->getSocket()->write(output.toJson());

    output.setObject(so);
    this->playerSouth->getSocket()->write(output.toJson());

    output.setObject(we);
    this->playerWest->getSocket()->write(output.toJson());
    qDebug()
            //<< "Discard: " << b->discard.size()     << "\n"
            << "South: "   << no << "\n"
            << "East: "    << ea << "\n"
            << "North: "   << so << "\n"
            << "West: "    << we << "\n"
            << "blind board status" << this->board->blindBoardStatus() << "\n"
               ;

}

QJsonObject Server::gameStart()
{
    QJsonObject obj;

    if (this->board->ingame.size() == 0)
    {
    }
    else if (this->board->ingame.size() == (FINISH_DEAL*NB_PLAYER))
    {
    }
    else if (this->board->ingame.size() == ((START_DEAL+FINISH_DEAL)*NB_PLAYER))
    {
        this->board->dealCards(this->playerSockets.values(), START_DEAL);
        obj.insert(JSON_command, JSON_game_start);
        this->appendAndSendCompleteBoardStatus(obj);
        // now wait for players announces (even if none)
    }

    return (obj);
}

QJsonObject Server::announced(QJsonObject announce)
{
    if (announce.contains(JSON_error))
        return (announce);

    QString player = announce.value(JSON_player).toString();
    QString req = announce.value(JSON_announce).toString();

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
            this->exchanged(announce);
            // start game for test... but missing exchange parts
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
    QJsonObject ex;
    ex.insert(JSON_command, JSON_exchange);

    if (o.value(JSON_command).toString() == JSON_announce)
    { // initiation of exchange protocol by end of dealing and mandatory announcment
        foreach (Player *p, this->playerSockets.values())
        {
            QJsonObject cards;
            cards.insert(JSON_give_left, JSON_none);
            cards.insert(JSON_give_friend, JSON_none);
            cards.insert(JSON_give_right, JSON_none);

            ex.insert(p->getName(), cards);
        }
        this->broadCast(ex);
        return ex;
    }
    else if (o.value(JSON_command).toString() == JSON_exchange)
    {
        bool allOk = true;
        QJsonObject cards;
        foreach (Player *p, this->playerSockets.values())
        {
            cards.insert(JSON_give_left, JSON_none);
            cards.insert(JSON_give_friend, JSON_none);
            cards.insert(JSON_give_right, JSON_none);

            if (p->left.state == used)
                cards.insert(JSON_give_left, JSON_ok);
            else
                allOk = false;
            if (p->front.state == used)
                cards.insert(JSON_give_friend, JSON_ok);
            else
                allOk = false;
            if (p->right.state == used)
                cards.insert(JSON_give_right, JSON_ok);
            else
                allOk = false;
            ex.insert(p->getName(), cards);
        }
        this->broadCast(ex);

        if (allOk)
        {
            this->board->setExchange();

            QJsonDocument output;
            foreach (Player *p, this->playerSockets.values())
            {
                QJsonObject playerCards = cards;

                std::vector<Card> front;
                front.push_back(p->front);
                std::vector<Card> right;
                right.push_back(p->right);
                std::vector<Card> left;
                left.push_back(p->left);

                playerCards.insert(JSON_receive_front, Board::encodeCardList(front));
                playerCards.insert(JSON_receive_right, Board::encodeCardList(right));
                playerCards.insert(JSON_receive_left , Board::encodeCardList(left));


                output.setObject(playerCards);
                p->getSocket()->write(output.toJson());
            }
            // all cards are dealt, game can begin
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

    }
    return ex;
}

QJsonObject Server::playerTurn(e_turn turn)
{
    QJsonObject playerTurn;

    playerTurn.insert(JSON_command, JSON_player_turn);
    playerTurn.insert(JSON_player_turn, this->board->playerPosition[turn]->playerPositionName);
    playerTurn.insert(JSON_player, this->board->playerPosition[turn]->getName());

    this->appendAndSendCompleteBoardStatus(playerTurn);

    return (playerTurn);
}

QJsonObject Server::cardPlayed(QJsonObject o)
{

    // TODO : verify if played cards are playable over the last played hand
    // TODO : if yes
    {
        // TODO : (Board object job) update board status, play cards technicaly
        this->turnFinished();
    }
    // TODO : if not
    // TODO :    show player RAGE!! insert error in 'QJsonObject o'

    this->broadCast(o); // show played cards even if badly played ^^ for the lulz

    return (o);
}

QJsonObject Server::checked(QJsonObject check)
{
    bool isPlayerTurn = this->board->currentPlayer()->getName() == check.value(JSON_player).toString();
    this->broadCast(check);

    if (isPlayerTurn && this->board->last_hand[0].value != e_card::dog)
        this->turnFinished();

    return (check);
}

QJsonObject Server::turnFinished()
{
    QJsonObject playerTurn;

    playerTurn.insert(JSON_command, JSON_end_turn);
    playerTurn.insert(JSON_player_turn, this->board->currentPlayer()->playerPositionName);
    playerTurn.insert(JSON_player, this->board->currentPlayer()->getName());
    playerTurn.insert(JSON_last_played, this->board->encodeCardList(this->board->last_hand));
    playerTurn.insert(JSON_leading_player, this->board->leader->getName());

    this->broadCast(playerTurn);

    if (!this->board->isGameOver())
    {
        this->board->nextTurn();

        if (this->board->last_hand[0].value == dog)
            this->board->nextTurn();

        while (this->board->currentPlayer()->hand.size() == 0)
            this->board->nextTurn();
        this->playerTurn(this->board->turn);
    }
    else
        this->endGame();

    return (playerTurn);
}

QJsonObject Server::endGame()
{
    QJsonObject endGame;

    endGame.insert(JSON_command, JSON_end_game);
    endGame.insert(JSON_vertical_team, this->board->verticalTeamScore);
    endGame.insert(JSON_horizontal_team, this->board->horizontalTeamScore);

    if ((this->board->verticalTeamScore >= this->board->maxScore) ||
            (this->board->horizontalTeamScore >= this->board->maxScore))
    {
        if (this->board->verticalTeamScore > this->board->horizontalTeamScore)
            endGame.insert(JSON_winner, JSON_vertical_team);
        else if (this->board->verticalTeamScore < this->board->horizontalTeamScore)
            endGame.insert(JSON_winner, JSON_horizontal_team);
        else
            endGame.insert(JSON_winner, JSON_ex_equo);
        this->board->resetBoard();
        this->broadCast(endGame);
    }
    else
    {

    }
    return (endGame);
}

QJsonObject Server::sendError(QJsonObject o)
{
    this->broadCast(o);
    return (o);
}

