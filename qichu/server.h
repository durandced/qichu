#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QObject>
#include <QSpinBox>
#include <QMap>

namespace Ui {
class Server;
}

class Server : public QWidget
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = 0);
    ~Server();

private slots:
    void playerTeamSelect(int teamNum);
    void on_start_clicked();

private:
    QStringList players;
    bool addPlayer(QString name);


    QMap<QString, int> teams;
    // int createGame(QStringList team1, QStringList team2);
    //

private:
    Ui::Server *ui;
};

#endif // SERVER_H
