#include "client.h"
#include "ui_client.h"

void Client::welcomed(QJsonObject o)
{

}

void Client::chatUpdate(QJsonObject o)
{
    if (o.contains(JSON_text))
        ui->log->append(o.value(JSON_text).toString());
}

void Client::gameStart(QJsonObject o)
{


}

void Client::playerTurn(QJsonObject o)
{


}

void Client::announced(QJsonObject o)
{


}

void Client::exchanged(QJsonObject o)
{


}

void Client::cardPlayed(QJsonObject o)
{


}

void Client::checked(QJsonObject o)
{


}

void Client::turnFinished(QJsonObject o)
{


}

void Client::endGame(QJsonObject o)
{


}

void Client::error(QJsonObject o)
{


}
