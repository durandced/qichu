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
//    this->broadCast(handShake);
    socket->write(output.toJson());
    this->updateHandshake();
#ifdef _DEBUG
    ui->log->append("send:");
    ui->log->append(output.toJson());
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
    ui->log->append(text);
    chat.insert(JSON_text, text);

    return this->chatUpdate(chat);
}

QJsonObject Server::announce(QJsonObject announce, Player *player)
{
    if (!announce.contains(JSON_announce))
        announce.insert(JSON_error, JSON_error_field);
    else if ((announce.value(JSON_announce).toString() != JSON_none) &&
             (announce.value(JSON_announce).toString() != JSON_tichu) &&
             (announce.value(JSON_announce).toString() != JSON_grand_tichu) &&
             (announce.value(JSON_announce).toString() != JSON_artichette))
        announce.insert(JSON_error, JSON_error_field);
    announce.insert(JSON_player, player->getName());
    return this->announced(announce);
}

QJsonObject Server::exchange(QJsonObject exchange, Player *player)
{

}

QJsonObject Server::playCards(QJsonObject cards, Player *player)
{

}

QJsonObject Server::check(QJsonObject check, Player *player)
{
    check.insert(JSON_player, player->getName());
    return this->checked(check);
}

QJsonObject Server::commandError(QJsonObject cmd, Player *player)
{

}
