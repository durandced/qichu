#include "client.h"
#include "ui_client.h"

Client::Client(QWidget *parent, QString host, int port, QString name, QString serverPass) :
    QDialog(parent), serverHost(host), serverPort(port), playerName(name), serverPassword(serverPass),
    ui(new Ui::Client)
{
    ui->setupUi(this);

    this->socket = new QTcpSocket(this);
    connect(this->socket, &QTcpSocket::connected,    this, &Client::connected);
    connect(this->socket, &QTcpSocket::disconnected, this, &Client::disconnected);
    connect(this->socket, &QTcpSocket::readyRead,    this, &Client::readyRead);
    connect(this->socket, &QTcpSocket::bytesWritten, this, &Client::bytesWritten);

    this->socket->connectToHost(host, port);

}

Client::~Client()
{
    delete ui;
}

void Client::connected()
{
    QString handShake;
    handShake = QString("%1:%2\n").arg(this->playerName).arg(this->serverPassword);
    this->socket->write(QByteArray(handShake.toStdString().c_str(), handShake.size()));
}

void Client::disconnected()
{

}

void Client::bytesWritten(qint64 bytes)
{

}

void Client::readyRead()
{
    ui->log->append(QString(this->socket->readAll()));
}

void Client::on_playButton_clicked()
{

}

void Client::on_closeButton_clicked()
{

}
