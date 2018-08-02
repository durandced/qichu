#include "server.h"
#include "ui_server.h"

// methodes are callbacks triggered by client command
// see this as command inputs

QJsonDocument Server::chat(QTcpSocket *socket, QJsonObject chat)
{
    QJsonDocument output;
    if (!chat.contains(JSON_text))
    {
        chat.insert(JSON_error, JSON_no_text);
        output.setObject(chat);
        return output;
    }
    QString text;
    text = this->playerSockets[socket]->getName() + " : " + chat.value(JSON_text).toString();
    ui->log->append(text);
    chat.insert(JSON_text, text);
    output.setObject(chat);
    foreach (Player* p, this->playerSockets)
        if (p->getSocket() != socket)
            p->getSocket()->write(output.toJson());
    return output;
}

QJsonDocument Server::announce(QJsonObject announce, Player *player)
{

}

QJsonDocument Server::exchange(QJsonObject announce, Player *player)
{

}

QJsonDocument Server::playCards(QJsonObject cards, Player *player)
{

}

QJsonDocument Server::check(QJsonObject check, Player *player)
{

}

QJsonDocument Server::error(QJsonObject cmd, Player *player)
{

}
