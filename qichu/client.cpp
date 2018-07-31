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
    if (this->socket->isOpen())
    {
        this->socket->close();
        delete this->socket;
    }
    delete ui;
}

void Client::connected()
{
    QJsonDocument handShake;
    QJsonObject o;
    o.insert("command", "handshake");
    o.insert("name", this->playerName);
    if (!this->serverPassword.isEmpty())
        o.insert("password", this->serverPassword);
    handShake.setObject(o);
    this->socket->write(handShake.toJson());
}

void Client::disconnected()
{
    ui->log->append("connection closed by server");
    QTimer wait;
    QEventLoop loop;
    wait.setInterval(2000);
    wait.setSingleShot(true);
    connect(&wait, &QTimer::timeout, this, &Client::hide);
    connect(&wait, &QTimer::timeout, this, &Client::close);
    connect(&wait, &QTimer::timeout, this, &Client::deleteLater);
    connect(&wait, &QTimer::timeout, &loop, &QEventLoop::quit);
    wait.start();
    loop.exec();
}

void Client::bytesWritten(qint64 bytes)
{

}

void Client::readyRead()
{
    ui->log->append(QString(this->socket->readAll()));
}

void Client::on_closeButton_clicked()
{
    this->close();
}

void Client::on_sendButton_clicked()
{
    if (ui->chat->text().isEmpty())
        return;
    QJsonDocument chat;
    QJsonObject o;
    o.insert("command", "chat");
    o.insert("text", ui->chat->text());
    ui->chat->setText("");
    chat.setObject(o);
    this->socket->write(chat.toJson());
}
