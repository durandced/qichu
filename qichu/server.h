#ifndef SERVER_H
#define SERVER_H

#include <QWidget>

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
    void playerTeamSelect();
    void on_start_clicked();

private:
    QStringList players;

    QStringList team0;
    QStringList team1;
    QStringList team2;

    QList<QStringList> teams;
    // int createGame(QStringList team1, QStringList team2);
    //

private:
    Ui::Server *ui;
};

#endif // SERVER_H
