#include "client.h"
#include "ui_client.h"

Client::Client(QWidget *parent, QString host, int port, QString name, QString serverPass) :
    QDialog(parent), serverHost(host), serverPort(port), playerName(name), serverPassword(serverPass),
    ui(new Ui::Client)
{
    ui->setupUi(this);
    connect(ui->chat, &QLineEdit::returnPressed, this, &Client::on_sendChat_clicked);
    ui->chat->setFocus();

    this->socket = new QTcpSocket(this);
    connect(this->socket, &QTcpSocket::connected,    this, &Client::connected);
    connect(this->socket, &QTcpSocket::disconnected, this, &Client::disconnected);
    connect(this->socket, &QTcpSocket::readyRead,    this, &Client::readyRead);
    connect(this->socket, &QTcpSocket::bytesWritten, this, &Client::bytesWritten);

    // TODO try to bind host to detect bad host names
    this->socket->connectToHost(host, port);
}

Client::~Client()
{
    if (this->socket != NULL)
    {
        if (this->socket->isOpen())
        {
            disconnect(this->socket, &QTcpSocket::connected,    this, &Client::connected);
            disconnect(this->socket, &QTcpSocket::disconnected, this, &Client::disconnected);
            disconnect(this->socket, &QTcpSocket::readyRead,    this, &Client::readyRead);
            disconnect(this->socket, &QTcpSocket::bytesWritten, this, &Client::bytesWritten);
            this->socket->close();
        }
    }
    delete ui;
}

void Client::connected()
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

void Client::disconnected()
{
    ui->log->append("connection closed by server");
}

void Client::bytesWritten(qint64 bytes)
{

}

void Client::readyRead()
{
    QJsonDocument input = QJsonDocument::fromJson(socket->readAll());
#ifdef _DEBUG
    ui->log->append("receive:");
    ui->log->append(input.toJson());
#endif
    QJsonObject o = input.object();
    if (o.value(JSON_command).toString() == JSON_chat)
        this->chatUpdate(o);

    else if (o.value(JSON_command).toString() == JSON_player_turn)
        this->playerTurn(o);
    else if (o.value(JSON_command).toString() == JSON_announce)
        this->announced(o);
    else if (o.value(JSON_command).toString() == JSON_exchange)
        this->exchanged(o);
    else if (o.value(JSON_command).toString() == JSON_play_cards)
        this->cardPlayed(o);
    else if (o.value(JSON_command).toString() == JSON_check)
        this->checked(o);
    else if (o.value(JSON_command).toString() == JSON_end_turn)
        this->turnFinished(o);
    else
        this->error(o);
}

void Client::on_closeButton_clicked()
{
    this->close();
}

void Client::chatUpdate(QJsonObject o)
{
    if (o.contains(JSON_text))
        ui->log->append(o.value(JSON_text).toString());
}

void Client::on_sendChat_clicked()
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

