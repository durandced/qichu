#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QObject>
#include <QSpinBox>
#include <QMap>
#include <QTcpServer>
#include <QTcpSocket>

namespace Ui {
class Server;
}

class Server : public QDialog
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = 0, int port = 2000);
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
    QStringList players;
    bool addPlayer(QString name);
    QTcpServer *tcpServer = NULL;

    QMap<QString, QTcpSocket*> playerSockets;

    QMap<QString, int> teams;
    // int createGame(QStringList team1, QStringList team2);
    //

private:
    Ui::Server *ui;
};

#endif // SERVER_H
