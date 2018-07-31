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

    this->tcpServer = new QTcpServer(this);

    connect(this->tcpServer, &QTcpServer::newConnection, this, &Server::newClient);

    this->tcpServer->listen(QHostAddress::Any, this->serverPort);

    ui->start->setEnabled(false);

    this->addPlayer("pl1");
    this->addPlayer("p123");
    this->addPlayer("pl455");
}

Server::~Server()
{
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
        foreach (int t, this->teams)
        {
            if (t == 1)
                t1++;
            if (t == 2)
                t2++;
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
    if (this->players.contains(name))
        return false;
    if (this->players.size() >= 4)
        return false;
    this->players.append(name);
    if (this->players.size() >= 1)
        ui->player0->setText(this->players[0]);
    if (this->players.size() >= 2)
        ui->player1->setText(this->players[1]);
    if (this->players.size() >= 3)
        ui->player2->setText(this->players[2]);
    if (this->players.size() >= 4)
        ui->player3->setText(this->players[3]);
    return true;
}


void Server::disconnected()
{

}

void Server::bytesWritten(qint64 bytes)
{

}

void Server::readyRead()
{
    QTcpSocket *socket = (QTcpSocket *)(QObject::sender());
    QJsonDocument input = QJsonDocument::fromJson(socket->readAll());
//    while (input.contains('\n'))
//        input.remove(input.indexOf('\n'), 1);
//    while (input.contains('\r'))
//        input.remove(input.indexOf('\r'), 1);

    if (this->playerSockets.values().contains(socket))
    { // known player

    }
    else
    {
        QJsonObject handShake = input.object();
        QString name;
        QString pass = "";
        if (handShake.contains("name"))
        {
            name = handShake.value("name").toString();
            handShake.remove("name");
            if (this->serverPassword.isEmpty() == false)
            {
                if (handShake.contains("password"))
                {
                    pass = handShake.value("password").toString();
                    handShake.remove("password");
                }
            }
            if (pass == this->serverPassword)
            {
                if (this->addPlayer(name))
                {
                    this->playerSockets[name] = socket;
                    handShake.insert("welcome", name);
                }
                else
                    handShake.insert("error", "wrong name");
            }
            else
                handShake.insert("error", "wrong password");
        }
        else
            handShake.insert("error", "no name");


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
        socket->write(output.toJson());
        if (handShake.contains("error"))
            socket->close();

    }
}
