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

    // TODO try to bind host to detect bad host names
    this->socket->connectToHost(host, port);
    ui->sendButton->setFocus();

}

Client::~Client()
{
    if (this->socket != NULL)
    {
        if (this->socket->isOpen())
            this->socket->close();
        delete this->socket;
        this->socket = NULL;
    }
    delete ui;
}

void Client::connected()
{
    QJsonDocument handShake;
    QJsonObject o;
    o.insert(JSON_command, JSON_handshake);
    o.insert(JSON_name, this->playerName);
    if (!this->serverPassword.isEmpty())
        o.insert(JSON_password, this->serverPassword);
    handShake.setObject(o);
#ifdef _DEBUG
    ui->log->append("send:");
    ui->log->append(handShake.toJson());
#endif
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
    QJsonDocument input = QJsonDocument::fromJson(socket->readAll());
#ifdef _DEBUG
    ui->log->append("receive:");
    ui->log->append(input.toJson());
#endif
    QJsonDocument output;
    QJsonObject o = input.object();
    if (o.value(JSON_command).toString() == JSON_chat)
        this->chat(o);
}

void Client::chat(QJsonObject o)
{
    if (o.contains(JSON_text))
    ui->log->append(o.value(JSON_text).toString());
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
    o.insert(JSON_command, JSON_chat);
    o.insert(JSON_text, ui->chat->text());
    ui->chat->setText("");
    chat.setObject(o);
#ifdef _DEBUG
    ui->log->append("send:");
    ui->log->append(chat.toJson());
#endif
    this->socket->write(chat.toJson());
}
