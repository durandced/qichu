#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QSpinBox>
#include <QMap>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QNetworkAddressEntry>

#include "player.h"
#include "protocol.h"
#include "board.h"

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
signals:
    void closed();
    void log(QString s);
// ////////////////////////////////////////////
// client/server members
// ////////////////////////////////////////////
private slots:
    void disconnected();
    void bytesWritten(qint64);
    void readyRead();
private:
    int                        serverPort;
    QTcpServer                 *tcpServer = nullptr;

// ////////////////////////////////////////////
// game members
// ////////////////////////////////////////////
private:          // players managment
    QString                    serverPassword;
    QMap<QString, QTcpSocket*> players;
    QMap<QTcpSocket*, Player*> playerSockets;

    e_startStage game_stage = e_startStage::gameNotStarted;
    Board *board;
    Player* playerNorth;
    Player* playerEast;
    Player* playerSouth;
    Player* playerWest;
    QMap<QString, int>         teams;
    bool addPlayer(QString name, QTcpSocket *socket);
    bool removePlayer(QString name);
    void broadCast(QJsonObject message);

private slots:    // ui methodes
    void on_start_clicked();
    void newClient();
    void on_switchSE_clicked();
    void on_switchEN_clicked();
    void on_switchNW_clicked();
    void on_switchWS_clicked();

private:          // basic protocol commands
    QJsonObject handShake(QTcpSocket *socket, QJsonObject handShake);
    QJsonObject chat(QTcpSocket *socket, QJsonObject chat);
    QJsonObject sendError(QJsonObject o);
    QJsonObject chatUpdate(QJsonObject chat);

private:          // game server commands
    QJsonObject announce(QJsonObject announce, Player *player);
    QJsonObject exchange(QJsonObject exchange, Player *player);
    QJsonObject playCards(QJsonObject cards, Player *player);
    QJsonObject check(QJsonObject check, Player *player);
    QJsonObject commandError(QJsonObject cmd, Player *player);
    QJsonObject appendAndSendCompleteBoardStatus(QJsonObject command);

private:          // game rules reaction (clients callback and server actions)
    void updateHandshake();
    void firstStageAnnounces();
    void dealFirstStage();
    void secondStageAnnounces();
    void dealSecondStage();
    void exchangeStage();
    QJsonObject gameStart();
    QJsonObject playerTurn(e_turn turn);
    QJsonObject announced(QJsonObject announce);
    QJsonObject exchanged(QJsonObject o);
    QJsonObject cardPlayed(QJsonObject o);
    QJsonObject checked(QJsonObject check);
    QJsonObject turnFinished();
    QJsonObject endGame();

protected:
    //void closeEvent(QCloseEvent * e);

private:
    Ui::Server *ui;
};

#endif // SERVER_H
