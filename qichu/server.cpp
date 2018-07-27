#include "server.h"
#include "ui_server.h"

Server::Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);
    connect(ui->teamP0, SIGNAL(valueChanged(int)), this, SLOT(playerTeamSelect(int)));
    connect(ui->teamP1, SIGNAL(valueChanged(int)), this, SLOT(playerTeamSelect(int)));
    connect(ui->teamP2, SIGNAL(valueChanged(int)), this, SLOT(playerTeamSelect(int)));
    connect(ui->teamP3, SIGNAL(valueChanged(int)), this, SLOT(playerTeamSelect(int)));

    ui->start->setEnabled(false);

    this->addPlayer("pl1");
    this->addPlayer("p123");
    this->addPlayer("pl455");
    this->addPlayer("pl111111");
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
