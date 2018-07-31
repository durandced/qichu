#include "server.h"
#include "ui_server.h"

Server::Server(QWidget *parent, int port) :
    QDialog(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);
    connect(ui->teamP0, SIGNAL(valueChanged(int)), this, SLOT(playerTeamSelect(int)));
    connect(ui->teamP1, SIGNAL(valueChanged(int)), this, SLOT(playerTeamSelect(int)));
    connect(ui->teamP2, SIGNAL(valueChanged(int)), this, SLOT(playerTeamSelect(int)));
    connect(ui->teamP3, SIGNAL(valueChanged(int)), this, SLOT(playerTeamSelect(int)));

    this->tcpServer = new QTcpServer(this);

    connect(this->tcpServer, &QTcpServer::newConnection, this, &Server::newClient);

    this->tcpServer->listen(QHostAddress::Any, port);

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
    ui->log->append(QString(socket->readAll()));
    socket->write("hey what's your name\n");
    if (socket->waitForReadyRead(1000) == false)
    {
        ui->log->append("Error: "+socket->errorString());
    }
    else
    {
        QString name = socket->readAll().split(':').at(0);
        this->playerSockets[name] = socket;
        QString handShake = QString("hello %1\n").arg(name);
        socket->write(QByteArray(handShake.toStdString().c_str(), handShake.size()));

    }
}
