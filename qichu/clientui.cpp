#include "clientui.h"
#include "ui_client.h"

ClientUi::ClientUi(QWidget *parent, QString host, int port, QString name, QString serverPass) :
    QDialog(parent), serverHost(host), serverPort(port), playerName(name), serverPassword(serverPass),
    ui(new Ui::Client)
{
    ui->setupUi(this);
    this->setWindowTitle("qichu : " + name);
    // setup ui chat
    connect(ui->chat, &QLineEdit::returnPressed, this, &ClientUi::on_sendChat_clicked);
    ui->chat->setFocus();

    // setup ui announce
    ui->announceSelect->addItem(JSON_none);
    ui->announceSelect->addItem(JSON_tichu);
    ui->announceSelect->addItem(JSON_grand_tichu);
    ui->announceSelect->addItem(JSON_artichette);


    this->socket = new QTcpSocket(this);
    connect(this->socket, &QTcpSocket::connected,    this, &ClientUi::connected);
    connect(this->socket, &QTcpSocket::disconnected, this, &ClientUi::disconnected);
    connect(this->socket, &QTcpSocket::bytesWritten, this, &ClientUi::bytesWritten);

    // TODO try to bind host to detect bad host names
    this->socket->connectToHost(host, port);
}

ClientUi::~ClientUi()
{
    qDebug() << "deleting client";
    if (this->socket != nullptr)
    {
        if (this->socket->isOpen())
        {
            disconnect(this->socket, &QTcpSocket::connected,    this, &ClientUi::connected);
            disconnect(this->socket, &QTcpSocket::disconnected, this, &ClientUi::disconnected);
            disconnect(this->socket, &QTcpSocket::readyRead,    this, &ClientUi::readyRead);
            disconnect(this->socket, &QTcpSocket::bytesWritten, this, &ClientUi::bytesWritten);
            this->socket->close();
        }
    }
    delete ui;
}

void ClientUi::closeEvent(QCloseEvent * e)
{
    emit closed();
    QWidget::closeEvent(e);
}

void ClientUi::readyRead()
{
    QByteArray b = socket->readAll();
    QString message = QString::fromUtf8(b.data());
    QStringList messages = message.split("}{");
    for (int i = 0; i < messages.length(); ++i)
    {
        if (i < (messages.length()-2))
            messages[i].append('}');
        if (i > 0)
            messages[i].prepend('{');
        qDebug() << "SPLITED MESSAGE" << messages.at(i);
        QJsonDocument input = QJsonDocument::fromJson(messages.at(i).toUtf8());

#ifdef _DEBUG
        qDebug() << "got " << input.toJson();
        ui->log->append("receive:" + input.toJson());
#endif
        QJsonObject o = input.object();
        if (o.value(JSON_command).toString() == JSON_handshake)
            this->welcomed(o);
        else if (o.value(JSON_command).toString() == JSON_chat)
            this->chatUpdate(o);
        else if (o.value(JSON_command).toString() == JSON_game_start)
            this->gameStart(o);
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
        else if (o.value(JSON_command).toString() == JSON_end_game)
            this->endGame(o);
        else
            this->error(o);
    }
}

void ClientUi::bytesWritten(qint64 )
{

}

void ClientUi::connected()
{
    connect(this->socket, &QTcpSocket::readyRead,    this, &ClientUi::readyRead);

    this->sendHandshake();
}

void ClientUi::disconnected()
{
    ui->log->append("connection closed by server");
}

// /////////////////////////////
// ui functions
// /////////////////////////////

void ClientUi::on_sendChat_clicked()
{
    this->sendChat();
}

void ClientUi::on_sendAnnounce_clicked()
{
    this->annonce(ui->announceSelect->currentText());
}

void ClientUi::on_sendCheck_clicked()
{
    this->check();
}
