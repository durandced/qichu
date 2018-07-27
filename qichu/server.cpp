#include "server.h"
#include "ui_server.h"

Server::Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);
    connect(ui->p1t1, &QRadioButton::clicked, this, &Server::playerTeamSelect);
    connect(ui->p1t2, &QRadioButton::clicked, this, &Server::playerTeamSelect);
    connect(ui->p1t0, &QRadioButton::clicked, this, &Server::playerTeamSelect);
    connect(ui->p2t1, &QRadioButton::clicked, this, &Server::playerTeamSelect);
    connect(ui->p2t2, &QRadioButton::clicked, this, &Server::playerTeamSelect);
    connect(ui->p2t0, &QRadioButton::clicked, this, &Server::playerTeamSelect);
    connect(ui->p3t1, &QRadioButton::clicked, this, &Server::playerTeamSelect);
    connect(ui->p3t2, &QRadioButton::clicked, this, &Server::playerTeamSelect);
    connect(ui->p3t0, &QRadioButton::clicked, this, &Server::playerTeamSelect);
    connect(ui->p4t1, &QRadioButton::clicked, this, &Server::playerTeamSelect);
    connect(ui->p4t2, &QRadioButton::clicked, this, &Server::playerTeamSelect);
    connect(ui->p4t0, &QRadioButton::clicked, this, &Server::playerTeamSelect);

    this->team0.clear();
    this->team1.clear();
    this->team2.clear();
    this->players.clear();
    this->teams.clear();
    this->teams.append(this->team0);
    this->teams.append(this->team1);
    this->teams.append(this->team2);
}

Server::~Server()
{
    delete ui;
}

void Server::playerTeamSelect()
{
    QRadioButton *sel = (QRadioButton*)(QObject::sender());
    int playerNum = sel->property("player").toInt();
    int teamNum = sel->property("team").toInt();

    if (teamNum != 0)
    {
        if (this->teams.at(teamNum).size() < 2)
            (QRadioButton*)(this->findChild(QString("p%1t%2").arg(playerNum).arg(teamNum)));
    }
}

void Server::on_start_clicked()
{

}
