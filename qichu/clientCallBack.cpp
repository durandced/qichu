#include "clientui.h"
#include "ui_client.h"

void ClientUi::welcomed(QJsonObject o)
{
    if (o.contains(JSON_handshake))
        ui->log->append(o.value(JSON_text).toString());

}

void ClientUi::chatUpdate(QJsonObject o)
{
    if (o.contains(JSON_text))
        ui->log->append(o.value(JSON_text).toString());
}

void ClientUi::gameStart(QJsonObject )
{
    // if hand.size == 8
    //   start exchange ui

    // if hand.size == 14
    //   stop exchange ui
    //   wait player turn
}

void ClientUi::playerTurn(QJsonObject )
{
    // ui.showPlayerTurn player

}

void ClientUi::announced(QJsonObject announce)
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

void ClientUi::exchanged(QJsonObject o)
{


}

void ClientUi::cardPlayed(QJsonObject o)
{
    if (o.contains(JSON_error))
    {
        if (o.value(JSON_error).toString() == JSON_play_error)
            ui->log->append("player rage : playing " + o.value(JSON_visible).toString() + " fails");
        // show player rage
    }
    else
    {
        ui->log->append("player play " + o.value(JSON_announce).toString());
        // show player announce
    }

}

void ClientUi::checked(QJsonObject check)
{
    QString player;
    if (check.contains(JSON_player))
        player = check.value(JSON_player).toString();
    else
        player = "error";
    ui->log->append("player " + player + " check");
    // ui show player check
}

void ClientUi::turnFinished(QJsonObject o)
{


}

void ClientUi::endGame(QJsonObject o)
{


}

void ClientUi::error(QJsonObject o)
{


}
