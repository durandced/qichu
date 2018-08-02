#include "player.h"
#include "ui_player.h"

Player::Player(QWidget *parent, QString n, QTcpSocket *s) :
    QWidget(parent), socket(s), name(n),
    ui(new Ui::Player)
{
    ui->setupUi(this);
}

Player::~Player()
{
    delete ui;
}
