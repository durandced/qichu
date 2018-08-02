#include "mainwindow.h"
#include "board.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Board b;

    qDebug() << "Discard: " << b.discard.size() << "\n"
             << "South: " << b.south.hand.size() << "\n"
             << "East: " << b.east.hand.size() << "\n    "
             << "North: " << b.north.hand.size() << "\n"
             << "West: " << b.west.hand.size() << "\n";
    return a.exec();
}
