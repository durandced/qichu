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
    // if hand.size == 9
    //   start exchange ui

    // if hand.size == 14
    //   stop exchange ui
    //   wait player turn
}

void Client::playerTurn(QJsonObject o)
{
    // ui.showPlayerTurn player

}

void Client::announced(QJsonObject announce)
{
    if (announce.contains(JSON_error))
    {
        if (announce.value(JSON_error).toString() == JSON_announce_error)
            ui->log->append("player rage : announce " + announce.value(JSON_announce).toString() + " fails");
        // show player rage
    }
    else
    {
        ui->log->append("player " + announce.value(JSON_announce).toString());
        // show player announce
    }
}

void Client::exchanged(QJsonObject o)
{


}

void Client::cardPlayed(QJsonObject o)
{


}

void Client::checked(QJsonObject check)
{
    QString player;
    if (check.contains(JSON_player))
        player = check.value(JSON_player).toString();
    else
        player = "error";
    ui->log->append("player " + player + " check");
    // ui show player check
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
