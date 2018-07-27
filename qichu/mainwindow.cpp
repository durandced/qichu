#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->server.hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_create_clicked()
{
    this->server.show();
}

void MainWindow::on_join_clicked()
{

}
