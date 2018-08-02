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
    void broadCast(QJsonObject message);

private slots:    // ui methodes
    void playerTeamSelect(int teamNum);
    void on_start_clicked();
    void newClient();

private:          // basic protocol commands
    QJsonObject handShake(QTcpSocket *socket, QJsonObject handShake);
    QJsonObject chat(QTcpSocket *socket, QJsonObject chat);

private:          // game server commands
    QJsonObject announce(QJsonObject announce, Player *player);
    QJsonObject exchange(QJsonObject announce, Player *player);
    QJsonObject playCards(QJsonObject cards, Player *player);
    QJsonObject check(QJsonObject check, Player *player);
    QJsonObject commandError(QJsonObject cmd, Player *player);

private:
    QJsonObject chatUpdate(QJsonObject chat);
    QJsonObject gameStart(QJsonObject o);
    QJsonObject playerTurn(QJsonObject o);
    QJsonObject announced(QJsonObject o);
    QJsonObject exchanged(QJsonObject o);
    QJsonObject cardPlayed(QJsonObject o);
    QJsonObject checked(QJsonObject o);
    QJsonObject turnFinished(QJsonObject o);
    QJsonObject endGame(QJsonObject o);
    QJsonObject sendError(QJsonObject o);

protected:
    void closeEvent(QCloseEvent * e);

private:
    Ui::Server *ui;
};

#endif // SERVER_H
