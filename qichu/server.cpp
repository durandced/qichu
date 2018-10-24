#include "server.h"
#include "ui_server.h"

Server::Server(QWidget *parent, int port, QString serverPass) :
    QDialog(parent), serverPassword(serverPass), serverPort(port),
    ui(new Ui::Server)
{
    ui->setupUi(this);
    ui->playerNorth->setText("waiting...");
    ui->playerEast->setText("waiting...");
    ui->playerSouth->setText("waiting...");
    ui->playerWest->setText("waiting...");

    ui->switchSE->setEnabled(false);
    ui->switchEN->setEnabled(false);
    ui->switchNW->setEnabled(false);
    ui->switchWS->setEnabled(false);
    ui->start->setEnabled(false);
    this->board = NULL;
    this->tcpServer = new QTcpServer(this);
    connect(this->tcpServer, &QTcpServer::newConnection, this, &Server::newClient);

    // TODO check listen return
    this->tcpServer->listen(QHostAddress::Any, this->serverPort);


    foreach (QNetworkInterface i, QNetworkInterface::allInterfaces())
        if (i.humanReadableName().contains("local"))
            if (i.addressEntries().size() >= 2)
                ui->log->append("Game server available at " + i.addressEntries().at(1).ip().toString()
                                + " port " + QString::number(this->serverPort));

}

void Server::closeEvent(QCloseEvent * e)
{
    emit closed();
    QWidget::closeEvent(e);
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

void Server::broadCast(QJsonObject message)
{
    QJsonDocument output;
    output.setObject(message);

    foreach (Player* p, this->playerSockets)
            p->getSocket()->write(output.toJson());
}

void Server::on_start_clicked()
{
    Player *north = this->playerNorth;
    Player *east = this->playerEast;
    Player *south = this->playerSouth;
    Player *west = this->playerWest;

    if (this->board == NULL)
        this->board = new Board(north, east, south, west, 1000);

    // deal 8 cards and wait players announces
    this->gameStart();

    qDebug()
            //<< "Discard: " << b->discard.size()     << "\n"
             << "South: "   << board->encodeCardList(south->hand) << "\n"
             << "East: "    << board->encodeCardList(east->hand) << "\n"
             << "North: "   << board->encodeCardList(north->hand) << "\n"
             << "West: "    << board->encodeCardList(west->hand) << "\n"
             << "blind board status" << this->board->blindBoardStatus() << "\n"
                ;

    ui->switchSE->setEnabled(false);
    ui->switchEN->setEnabled(false);
    ui->switchNW->setEnabled(false);
    ui->switchWS->setEnabled(false);
    ui->start->setEnabled(false);
}

void Server::newClient()
{
    QTcpSocket *socket = this->tcpServer->nextPendingConnection();

    connect(socket, &QTcpSocket::disconnected, this, &Server::disconnected);
    connect(socket, &QTcpSocket::readyRead,    this, &Server::readyRead);
    connect(socket, &QTcpSocket::bytesWritten, this, &Server::bytesWritten);
}

bool Server::addPlayer(QString name, QTcpSocket* socket)
{
    bool ret = false;
    if (this->players.size() >= 4)
        return ret;

    ret = true;
    if (this->players.keys().contains(name))
        ret = false;
    else
    {
        this->playerSockets[socket] = new Player(this, name, socket);
        this->players[name] = socket;
    }

    this->playerNorth = NULL;
    this->playerEast = NULL;
    this->playerSouth = NULL;
    this->playerWest = NULL;
    if (this->players.keys().size() >= 1)
    {
        ui->playerNorth->setText(this->players.keys()[0]);
        this->playerNorth = this->playerSockets[this->players[this->players.keys()[0]]];
    }
    if (this->players.keys().size() >= 2)
    {
        ui->playerEast->setText(this->players.keys()[1]);
        this->playerEast = this->playerSockets[this->players[this->players.keys()[1]]];
    }
    if (this->players.keys().size() >= 3)
    {
        ui->playerSouth->setText(this->players.keys()[2]);
        this->playerSouth = this->playerSockets[this->players[this->players.keys()[2]]];
    }
    if (this->players.keys().size() >= 4)
    {
        ui->playerWest->setText(this->players.keys()[3]);
        this->playerWest = this->playerSockets[this->players[this->players.keys()[3]]];
    }

    if (this->players.keys().size() == 4)
    {
        ui->switchSE->setEnabled(true);
        ui->switchEN->setEnabled(true);
        ui->switchNW->setEnabled(true);
        ui->switchWS->setEnabled(true);
        ui->start->setEnabled(true);

    }

    return ret;
}

bool Server::removePlayer(QString name)
{
    if (this->players.contains(name))
        this->players.remove(name);
    else
        return false;

    ui->playerNorth->setText("waiting...");
    ui->playerEast->setText("waiting...");
    ui->playerSouth->setText("waiting...");
    ui->playerWest->setText("waiting...");

    foreach (QString player, this->players.keys())
        this->addPlayer(player, this->players[player]);

    ui->switchSE->setEnabled(false);
    ui->switchEN->setEnabled(false);
    ui->switchNW->setEnabled(false);
    ui->switchWS->setEnabled(false);
    ui->start->setEnabled(false);

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

void Server::bytesWritten(qint64 )
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
    QJsonObject o = input.object();
    if (this->playerSockets.contains(socket))
    { // known player
        Player *player = this->playerSockets[socket];
        if (o.value(JSON_command).toString() == JSON_chat)
            o = this->chat(socket, o);
        else if (o.value(JSON_command).toString() == JSON_announce)
            o = this->announce(o, player);
        else if (o.value(JSON_command).toString() == JSON_exchange)
            o = this->exchange(o, player);
        else if (o.value(JSON_command).toString() == JSON_play_cards)
            o = this->playCards(o, player);
        else if (o.value(JSON_command).toString() == JSON_check)
            o = this->check(o, player);
        else
            o = this->commandError(o, player);
    }
    else
    {
        if (o.value(JSON_command).toString() == JSON_handshake)
            o = this->handShake(socket, o);
        else
            o = this->commandError(o, NULL);
    }
#ifdef _DEBUG
    QJsonDocument output;
    output.setObject(o);
    ui->log->append("callback output:");
    ui->log->append(output.toJson());
#endif
}

void Server::on_switchNW_clicked()
{
    QString w = ui->playerWest->text();
    QString n = ui->playerNorth->text();

    ui->playerWest->setText(n);
    ui->playerNorth->setText(w);

    Player* pw = this->playerWest;
    Player* pn = this->playerNorth;

    this->playerWest = pn;
    this->playerNorth = pw;
    this->updateHandshake();
}

void Server::on_switchEN_clicked()
{
    QString n = ui->playerNorth->text();
    QString e = ui->playerEast->text();

    ui->playerNorth->setText(e);
    ui->playerEast->setText(n);

    Player* pn = this->playerNorth;
    Player* pe = this->playerEast;

    this->playerEast = pn;
    this->playerNorth = pe;
    this->updateHandshake();
}

void Server::on_switchSE_clicked()
{
    QString e = ui->playerEast->text();
    QString s = ui->playerSouth->text();

    ui->playerEast->setText(s);
    ui->playerSouth->setText(e);

    Player* pe = playerEast;
    Player* ps = playerSouth;

    playerEast = ps;
    playerSouth = pe;
    this->updateHandshake();
}

void Server::on_switchWS_clicked()
{
    QString s = ui->playerSouth->text();
    QString w = ui->playerWest->text();

    ui->playerSouth->setText(w);
    ui->playerWest->setText(s);

    Player* ps = playerSouth;
    Player* pw = playerWest;

    playerSouth = pw;
    playerWest = ps;
    this->updateHandshake();
}
