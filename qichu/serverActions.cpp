#include "server.h"
#include "ui_server.h"

// methodes here are server actions. this are reaction after a client command have treated

QJsonObject Server::chatUpdate(QJsonObject chat)
{
    this->broadCast(chat);
    return (chat);
}

QJsonObject Server::gameStart(QJsonObject o)
{
    this->broadCast(o);
    return (o);
}

QJsonObject Server::playerTurn(QJsonObject o)
{
    this->broadCast(o);
    return (o);
}

QJsonObject Server::announced(QJsonObject o)
{
    this->broadCast(o);
    return (o);
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

QJsonObject Server::checked(QJsonObject o)
{
    this->broadCast(o);
    return (o);
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

