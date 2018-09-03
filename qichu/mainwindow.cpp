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
//    server.setModal(true); //exec();
    server.show();
    ui->ip->setText("localhost");
    this->on_join_clicked();
    //    this->show();
    // delete this->server;
    // this->server = NULL;
}

void MainWindow::on_join_clicked()
{
    QEventLoop loop;
    Client client(this, ui->ip->text(), ui->port->value(), ui->name->text(), ui->pass->text());
    Client bob(this, ui->ip->text(), ui->port->value(), "bob", ui->pass->text());
    Client timy(this, ui->ip->text(), ui->port->value(), "timy", ui->pass->text());
    Client rob(this, ui->ip->text(), ui->port->value(), "rob", ui->pass->text());

    connect(&client, &Client::closed, &loop, &QEventLoop::quit);
//    this->hide();
    client.show();
    bob.show();
    timy.show();
    rob.show();
    loop.exec();
//    this->show();

}
