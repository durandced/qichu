#include "server.h"
#include "ui_server.h"

Server::Server(QWidget *parent, int port, QString serverPass) :
    QDialog(parent), serverPassword(serverPass), serverPort(port),
    ui(new Ui::Server)
{
    ui->setupUi(this);
    connect(ui->teamP0, SIGNAL(valueChanged(int)), this, SLOT(playerTeamSelect(int)));
    connect(ui->teamP1, SIGNAL(valueChanged(int)), this, SLOT(playerTeamSelect(int)));
    connect(ui->teamP2, SIGNAL(valueChanged(int)), this, SLOT(playerTeamSelect(int)));
    connect(ui->teamP3, SIGNAL(valueChanged(int)), this, SLOT(playerTeamSelect(int)));
    ui->player0->setText("waiting...");
    ui->player1->setText("waiting...");
    ui->player2->setText("waiting...");
    ui->player3->setText("waiting...");

    this->tcpServer = new QTcpServer(this);
    connect(this->tcpServer, &QTcpServer::newConnection, this, &Server::newClient);

    // TODO check listen return
    this->tcpServer->listen(QHostAddress::Any, this->serverPort);


    foreach (QNetworkInterface i, QNetworkInterface::allInterfaces())
        if (i.humanReadableName().contains("local"))
            if (i.addressEntries().size() >= 2)
                ui->log->append("Game server available at " + i.addressEntries().at(1).ip().toString()
                                + " port " + QString::number(this->serverPort));

    ui->start->setEnabled(false);
}


void Server::closeEvent(QCloseEvent * e)
{

    QDialog::closeEvent(e);
}

Server::~Server()
{
    foreach (QTcpSocket *socket, this->playerSockets.keys())
    {
        if (socket->isOpen())
        {
            disconnect(socket, &QTcpSocket::disconnected, this, &Server::disconnected);
            disconnect(socket, &QTcpSocket::readyRead,    this, &Server::readyRead);
            disconnect(socket, &QTcpSocket::bytesWritten, this, &Server::bytesWritten);
            socket->close();
        }
    }
    disconnect(this->tcpServer, &QTcpServer::newConnection, this, &Server::newClient);
    this->tcpServer->close();
    delete ui;
}

void Server::playerTeamSelect(int teamNum)
{
    QSpinBox *sel = (QSpinBox*)(QObject::sender());
    int playerNum = sel->property("player").toInt();
    ui->start->setEnabled(false);

    if (playerNum < this->players.size())
        this->teams[this->players.at(playerNum)] = teamNum;

    int t1 = 0;
    int t2 = 0;
    if (this->teams.size() == 4)
    {
        foreach (int t, this->teams)
        {
            if (t == 1)
                t1++;
            if (t == 2)
                t2++;
        }
    }
    if (t1 == 2 && t2 == 2)
        ui->start->setEnabled(true);
}

void Server::on_start_clicked()
{

}

void Server::newClient()
{
    QTcpSocket *socket = this->tcpServer->nextPendingConnection();

    connect(socket, &QTcpSocket::disconnected, this, &Server::disconnected);
    connect(socket, &QTcpSocket::readyRead,    this, &Server::readyRead);
    connect(socket, &QTcpSocket::bytesWritten, this, &Server::bytesWritten);
}

bool Server::addPlayer(QString name)
{
    bool ret = false;
    if (this->players.size() >= 4)
        return ret;

    ret = true;
    if (this->players.contains(name))
        ret = false;
    else
        this->players.append(name);

    if (this->players.size() >= 1)
        ui->player0->setText(this->players[0]);
    if (this->players.size() >= 2)
        ui->player1->setText(this->players[1]);
    if (this->players.size() >= 3)
        ui->player2->setText(this->players[2]);
    if (this->players.size() >= 4)
        ui->player3->setText(this->players[3]);
    return ret;
}

bool Server::removePlayer(QString name)
{
    if (this->players.contains(name))
        this->players.removeAt(this->players.indexOf(name));
    else
        return false;

    ui->player0->setText("waiting...");
    ui->player1->setText("waiting...");
    ui->player2->setText("waiting...");
    ui->player3->setText("waiting...");

    foreach (QString player, this->players)
        this->addPlayer(player);

    return true;
}

void Server::disconnected()
{
    QTcpSocket *socket = (QTcpSocket *)(QObject::sender());
    ui->log->append("clien disconnected.");

    disconnect(socket, &QTcpSocket::disconnected, this, &Server::disconnected);
    disconnect(socket, &QTcpSocket::readyRead,    this, &Server::readyRead);
    disconnect(socket, &QTcpSocket::bytesWritten, this, &Server::bytesWritten);

    if (this->playerSockets.contains(socket))
    {
        QString name = this->playerSockets[socket]->getName();
        ui->log->append(name);

        this->removePlayer(name);
        delete this->playerSockets[socket];
        this->playerSockets.remove(socket);

        if (socket->isOpen())
            socket->close();
        delete socket;
    }
}

void Server::bytesWritten(qint64 bytes)
{

}

void Server::readyRead()
{
    QTcpSocket *socket = (QTcpSocket *)(QObject::sender());
    QJsonDocument input = QJsonDocument::fromJson(socket->readAll());
#ifdef _DEBUG
    ui->log->append("receive:");
    ui->log->append(input.toJson());
#endif
    QJsonDocument output;
    QJsonObject o = input.object();
    if (this->playerSockets.contains(socket))
    { // known player
        Player *player = this->playerSockets[socket];
        if (o.value(JSON_command).toString() == JSON_chat)
            output = this->chat(socket, o);

        else if (o.value(JSON_command).toString() == JSON_announce)
            output = this->announce(o, player);
        else if (o.value(JSON_command).toString() == JSON_exchange)
            output = this->exchange(o, player);
        else if (o.value(JSON_command).toString() == JSON_play_cards)
            output = this->playCards(o, player);
        else if (o.value(JSON_command).toString() == JSON_check)
            output = this->check(o, player);
        else
            output = this->error(o, player);
    }
    else
    {
        if (o.value(JSON_command).toString() == JSON_handshake)
            output = this->handShake(socket, o);
    }
#ifdef _DEBUG
    ui->log->append("send:");
    ui->log->append(output.toJson());
#endif

    socket->write(output.toJson());
}

QJsonDocument Server::handShake(QTcpSocket *socket, QJsonObject handShake)
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
            if (this->addPlayer(name))
            {
                this->playerSockets[socket] = new Player(this, name, socket);
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

    if (this->players.size() >= 1)
        handShake.insert("player0", this->players[0]);
    if (this->players.size() >= 2)
        handShake.insert("player1", this->players[1]);
    if (this->players.size() >= 3)
        handShake.insert("player2", this->players[2]);
    if (this->players.size() >= 4)
        handShake.insert("player3", this->players[3]);

    QJsonDocument output;
    output.setObject(handShake);
    if (handShake.contains(JSON_error))
        socket->close();

    return output;
}
