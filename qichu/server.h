#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QSpinBox>
#include <QMap>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>

#include "player.h"

namespace Ui {
class Server;
}

class Server : public QDialog
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = 0, int port = 2000, QString serverPass = "");
    ~Server();

private slots:
    void playerTeamSelect(int teamNum);
    void on_start_clicked();
    void newClient();
private slots:
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

private:
    QString serverPassword;
    int serverPort;
    QStringList players;
    bool addPlayer(QString name);
    bool removePlayer(QString name);
    QTcpServer *tcpServer = NULL;

    QMap<QTcpSocket*, Player*> playerSockets;

    QMap<QString, int> teams;
    // int createGame(QStringList team1, QStringList team2);
    //

private:
    QJsonDocument handShake(QTcpSocket *socket, QJsonObject handShake);
    QJsonDocument chat(QTcpSocket *socket, QJsonObject chat);


private:
    Ui::Server *ui;
};

#endif // SERVER_H
