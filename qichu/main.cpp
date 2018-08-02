#include "mainwindow.h"
#include <QApplication>
#include "board.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Player *north = new Player(NULL, "player0", NULL);
    Player *east = new Player(NULL, "player1", NULL);
    Player *south = new Player(NULL, "player2", NULL);
    Player *west = new Player(NULL, "player3", NULL);

    Board b(north, east, south, west);

    return a.exec();
}
