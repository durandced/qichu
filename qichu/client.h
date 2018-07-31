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
#include "core.h"

namespace Ui {
class Client;
}

class Client : public QDialog
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = 0, QString host = "localhost", int port = 2000, QString name = "toto", QString serverPass = "");
    ~Client();
private:
    QTcpSocket *socket;

public:

private:
    QString serverHost;
    int serverPort;
    QString playerName;
    QString serverPassword;

private:
    void chat(QJsonObject o);



private slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

private slots:
    void on_closeButton_clicked();
    void on_sendButton_clicked();

private:
    Ui::Client *ui;
};

#endif // CLIENT_H
