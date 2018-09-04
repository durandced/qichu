#include "player.h"
#include "ui_player.h"

Player::Player(QWidget *parent, QString n, QTcpSocket *s) :
    QWidget(parent), socket(s), name(n),
    ui(new Ui::Player)
{
    ui->setupUi(this);
}

Player::Player()
{

}

Player::~Player()
{
    delete ui;
}

int Player::count_points()
{
    int score = 0;
    for (auto &c : this->won)
    {
        score += c.points;
    }

    return score;
}
