#ifndef CLIENT_H
#define CLIENT_H

#include <QDialog>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <QEventLoop>
#include <QCloseEvent>

#include "player.h"
#include "protocol.h"

namespace Ui {
class Client;
}

class Client : public QDialog
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent    = 0,
                    QString host       = "localhost",
                    int     port       = 2000,
                    QString name       = "toto",
                    QString serverPass = "");
    ~Client();

// ////////////////////////////////////////////
// client/server members
// ////////////////////////////////////////////
private:
    QTcpSocket      *socket;
    QString         playerName;
    QString         serverHost;
    int             serverPort;
    QString         serverPassword;
private slots:
    void connected(); // handshake
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

// ////////////////////////////////////////////
// game members
// ////////////////////////////////////////////
private slots: // basics
    void on_sendChat_clicked();
    void on_sendAnnounce_clicked();

private: // player actions
    void sendHandshake();
    void sendChat();
    void annonce(QString announce);
    void exchange();
    void playCards();
    void check();

private: // game server messages/responses
    void welcomed(QJsonObject o);
    void chatUpdate(QJsonObject o);
    void gameStart(QJsonObject o);
    void playerTurn(QJsonObject o);
    void announced(QJsonObject announce);
    void exchanged(QJsonObject o);
    void cardPlayed(QJsonObject o);
    void checked(QJsonObject o);
    void turnFinished(QJsonObject o);
    void endGame(QJsonObject o);
    void error(QJsonObject o);

private:
    Ui::Client *ui;
};

#endif // CLIENT_H
