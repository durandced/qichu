#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "server.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_create_clicked();
    void on_join_clicked();

private:
    Server server;

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
