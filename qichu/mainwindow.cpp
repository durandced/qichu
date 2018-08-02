#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QString name = qgetenv("USER");
    if (name.isEmpty())
        name = qgetenv("USERNAME");
    qDebug() << name;
    ui->name->setText(name);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_create_clicked()
{

    Server server(this, ui->port->value(), ui->pass->text());
    //    this->hide();
    server.setModal(true); //exec();
    server.show();
    ui->ip->setText("localhost");
    this->on_join_clicked();
    //    this->show();
    // delete this->server;
    // this->server = NULL;
}

void MainWindow::on_join_clicked()
{
    Client client(this, ui->ip->text(), ui->port->value(), ui->name->text(), ui->pass->text());
    this->hide();
    client.exec();
    this->show();

}
