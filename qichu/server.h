#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QSpinBox>
#include <QMap>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QNetworkAddressEntry>

#include "player.h"
#include "protocol.h"

namespace Ui {
class Server;
}

class Server : public QDialog
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent    = 0,
                    int     port       = 2000,
                    QString serverPass = "");
    ~Server();

// ////////////////////////////////////////////
// client/server members
// ////////////////////////////////////////////
private slots:
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();
private:
    int                        serverPort;
    QTcpServer                 *tcpServer = NULL;

// ////////////////////////////////////////////
// game members
// ////////////////////////////////////////////
private:          // players managment
    QString                    serverPassword;
    QStringList                players;
    QMap<QTcpSocket*, Player*> playerSockets;
    QMap<QString, int>         teams;
    bool addPlayer(QString name);
    bool removePlayer(QString name);

private slots:    // ui methodes
    void playerTeamSelect(int teamNum);
    void on_start_clicked();
    void newClient();

private:          // basic protocol commands
    QJsonDocument handShake(QTcpSocket *socket, QJsonObject handShake);
    QJsonDocument chat(QTcpSocket *socket, QJsonObject chat);

private:          // game server commands
    QJsonDocument announce(QJsonObject announce, Player *player);
    QJsonDocument exchange(QJsonObject announce, Player *player);
    QJsonDocument playCards(QJsonObject cards, Player *player);
    QJsonDocument check(QJsonObject check, Player *player);
    QJsonDocument error(QJsonObject cmd, Player *player);

protected:
    void closeEvent(QCloseEvent * e);

private:
    Ui::Server *ui;
};

#endif // SERVER_H
