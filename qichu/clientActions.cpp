#include "clientui.h"
#include "ui_client.h"

void ClientUi::sendHandshake()
{
    QJsonDocument handShake;
    QJsonObject o;
    o.insert(JSON_command, JSON_handshake);
    o.insert(JSON_player, this->playerName);
    if (!this->serverPassword.isEmpty())
        o.insert(JSON_password, this->serverPassword);
    handShake.setObject(o);
#ifdef _DEBUG
    ui->log->append("send:");
    ui->log->append(handShake.toJson());
#endif
    this->socket->write(handShake.toJson());
}

void ClientUi::sendChat()
{
    if (ui->chat->text().isEmpty())
        return;
    QJsonDocument chat;
    QJsonObject o;
    o.insert(JSON_command, JSON_chat);
    o.insert(JSON_text, ui->chat->text());
    ui->chat->setText("");
    chat.setObject(o);
#ifdef _DEBUG
    ui->log->append("send:");
    ui->log->append(chat.toJson());
#endif
    this->socket->write(chat.toJson());
    ui->chat->setFocus();
}

void ClientUi::annonce(QString announce)
{
    QJsonDocument output;
    QJsonObject o;
    o.insert(JSON_command, JSON_announce);

    if (announce == JSON_none)
        o.insert(JSON_announce, announce);
    else if (announce == JSON_tichu)
        o.insert(JSON_announce, announce);
    else if (announce == JSON_grand_tichu)
        o.insert(JSON_announce, announce);
    else if (announce == JSON_artichette)
        o.insert(JSON_announce, announce);
    else
        return;
    output.setObject(o);
#ifdef _DEBUG
    ui->log->append("send:");
    ui->log->append(output.toJson());
#endif
    this->socket->write(output.toJson());
}

void ClientUi::exchange()
{
//    QJsonDocument exchange;
//    QJsonObject o;
//    o.insert(JSON_command, JSON_exchange);
//    // some stuff
//    std::vector<Card> front = {this->player->front}; especialy this! is shit
//    std::vector<Card> right = {this->player->right};
//    std::vector<Card> left = {this->player->left};
//
//    o.insert(JSON_give_friend, JSON_none);
//    o.insert(JSON_give_right, JSON_none);
//    o.insert(JSON_give_left, JSON_none);
//
//    if (this->player->front.state != unused)
//        o.insert(JSON_give_friend, Board::encodeCardList(front));
//    if (this->player->right.state != unused)
//        o.insert(JSON_give_friend, Board::encodeCardList(right));
//    if (this->player->left.state != unused)
//        o.insert(JSON_give_friend, Board::encodeCardList(left));
//
//    exchange.setObject(o);
//#ifdef _DEBUG
//    ui->log->append("send:");
//    ui->log->append(exchange.toJson());
//#endif
//    this->socket->write(exchange.toJson());
}

void ClientUi::playCards()
{
    QJsonDocument exchange;
    QJsonObject o;
    o.insert(JSON_command, JSON_exchange);
    // some stuff
    exchange.setObject(o);
#ifdef _DEBUG
    ui->log->append("send:");
    ui->log->append(exchange.toJson());
#endif
    this->socket->write(exchange.toJson());
}

void ClientUi::check()
{
    QJsonDocument check;
    QJsonObject o;
    o.insert(JSON_command, JSON_check);
    check.setObject(o);
#ifdef _DEBUG
    ui->log->append("send:");
    ui->log->append(check.toJson());
#endif
    this->socket->write(check.toJson());
}
