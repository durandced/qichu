#include "server.h"
#include "ui_server.h"

// methodes are callbacks triggered by client command
// see this as command inputs

QJsonObject Server::handShake(QTcpSocket *socket, QJsonObject handShake)
{
    QString name;
    QString pass = "";
    if (handShake.contains(JSON_player))
    {
        name = handShake.value(JSON_player).toString();
        handShake.remove(JSON_player);
        if (this->serverPassword.isEmpty() == false)
        {
            if (handShake.contains(JSON_password))
            {
                pass = handShake.value(JSON_password).toString();
                handShake.remove(JSON_password);
            }
        }
        if (pass == this->serverPassword)
        {
            if (this->addPlayer(name, socket))
            {
                handShake.insert(JSON_welcome, name);
            }
            else
                handShake.insert(JSON_error, JSON_wrong_name);
        }
        else
            handShake.insert(JSON_error, JSON_wrong_password);
    }
    else
        handShake.insert(JSON_error, JSON_no_name);

    if (handShake.contains(JSON_error))
    {
        socket->close();
        return handShake;
    }

    QJsonDocument output;
    output.setObject(handShake);
    this->broadCast(handShake);
    socket->write(output.toJson());
    this->updateHandshake();
#ifdef _DEBUG
    //ui->log->append("send:");
    //ui->log->append(output.toJson());
#endif
    return handShake;
}

QJsonObject Server::chat(QTcpSocket *socket, QJsonObject chat)
{
    if (!chat.contains(JSON_text))
    {
        chat.insert(JSON_error, JSON_no_text);
        return this->sendError(chat);
    }
    QString text;
    text = this->playerSockets[socket]->getName() + " : " + chat.value(JSON_text).toString();
    //ui->log->append(text);
    chat.insert(JSON_text, text);

    return this->chatUpdate(chat);
}

QJsonObject Server::announce(QJsonObject announce, Player *player)
{
    if (!announce.contains(JSON_announce))
    { // if no announce in packet
        announce.insert(JSON_error, JSON_error_field);
        announce.insert(JSON_player, player->getName());
        return this->announced(announce);
    }

    QString announceName = announce.value(JSON_announce).toString();
    // checking annonce name
    if ((announceName != JSON_none) &&
        (announceName != JSON_tichu) &&
        (announceName != JSON_grand_tichu) &&
        (announceName != JSON_artichette))
    { // unknown announce
        announce.insert(JSON_error, JSON_announce_error);
        announce.insert(JSON_player, player->getName());
        return this->announced(announce);
    }

    e_announce playerAnnounce = e_announce::unknown;

    if (announceName == JSON_none)
        playerAnnounce = e_announce::no;
    if (announceName == JSON_tichu)
        playerAnnounce = e_announce::tichu;
    if (announceName == JSON_grand_tichu)
        playerAnnounce = e_announce::grandTichu;
    if (announceName == JSON_artichette)
        playerAnnounce = e_announce::artichette;

    switch (this->game_stage)
    {
    case e_startStage::gameNotStarted:
    case e_startStage::firstStageAnnounce:
        // if game not started - player can say anything dumb
        // ARTICHETTE can only be a blind announce... 'cuz its for the ballzy one
        player->announce = playerAnnounce;
        player->announceName = announceName;
        break;
    case e_startStage::dealFirstStage:
    case e_startStage::secondStageAnnounce:
        // if game started and first stage deal is done
        if (playerAnnounce == e_announce::artichette)
        { // cannot accept artichette anymore, and cant go back
            announce.insert(JSON_error, JSON_announce_error);
            announce.insert(JSON_player, player->getName());
            return this->announced(announce);
        }
        player->announce = playerAnnounce;
        player->announceName = announceName;
        break;
    case e_startStage::dealSecondStage:
    case e_startStage::exchangeStage:
    case e_startStage::gameStarted:
        // if game started and first stage deal is done
        if ((playerAnnounce == e_announce::artichette) ||
                (playerAnnounce == e_announce::grandTichu) ||
                (playerAnnounce == e_announce::no))
        { // cannot accept artichette anymore, cant go back, and no gd tichu
            announce.insert(JSON_error, JSON_announce_error);
            announce.insert(JSON_player, player->getName());
            return this->announced(announce);
        }
        player->announce = playerAnnounce;
        player->announceName = announceName;
        break;
    default:
        break;
    }
    announce.insert(JSON_player, player->getName());
    return this->announced(announce);
}

QJsonObject Server::exchange(QJsonObject exchange, Player *player)
{
    if (exchange.contains(JSON_give_friend) &&
        exchange.contains(JSON_give_right) &&
        exchange.contains(JSON_give_left) )
    {
// ------------------------------------------------------------------------
        std::vector<Card> right;
        if (exchange.value(JSON_give_right).toString() == JSON_none)
            player->right.state = unused;
        else if (exchange.value(JSON_give_right).isArray())
        {
            right = this->board->decodeCardList(exchange.value(JSON_give_right).toArray());
            if (right.size() == 1)
            {
                player->right = right.at(0);
                player->right.state = used;
            }
            else
                exchange.insert(JSON_error, JSON_exchange_error);
        }
        else
            exchange.insert(JSON_error, JSON_exchange_error);

// ------------------------------------------------------------------------
        std::vector<Card> left;
        if (exchange.value(JSON_give_left).toString() == JSON_none)
            player->left.state = unused;
        else if (exchange.value(JSON_give_left).isArray())
        {
            left = this->board->decodeCardList(exchange.value(JSON_give_left).toArray());
            if (left.size() == 1)
            {
                player->left = left.at(0);
                player->left.state = used;
            }
            else
                exchange.insert(JSON_error, JSON_exchange_error);
        }
        else
            exchange.insert(JSON_error, JSON_exchange_error);

// ------------------------------------------------------------------------
        std::vector<Card> front;
        if (exchange.value(JSON_give_friend).toString() == JSON_none)
            player->front.state = unused;
        else if (exchange.value(JSON_give_friend).isArray())
        {
            front = this->board->decodeCardList(exchange.value(JSON_give_friend).toArray());
            if (front.size() == 1)
            {
                player->front = front.at(0);
                player->front.state = used;
            }
            else
                exchange.insert(JSON_error, JSON_exchange_error);
        }
        else
            exchange.insert(JSON_error, JSON_exchange_error);

    }
    else
        exchange.insert(JSON_error, JSON_exchange_error);

    return this->exchanged(exchange);
}

QJsonObject Server::playCards(QJsonObject cards, Player *player)
{
 // check card presence in player hand
    cards.insert(JSON_player, player->getName());
 //
}

QJsonObject Server::check(QJsonObject check, Player *player)
{
    // first game stages
    if (this->game_stage < e_startStage::gameStarted)
    {
        player->announce = e_announce::no;
        player->announceName = JSON_none;
    }

    check.insert(JSON_player, player->getName());
    return this->checked(check);
}

QJsonObject Server::commandError(QJsonObject cmd, Player *player)
{

}
