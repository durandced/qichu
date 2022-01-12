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
    ui->name->setText(name);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_create_clicked()
{

    //Server *server = new Server(this, ui->port->value(), ui->pass->text());
    //server->show();
    Server s(this, ui->port->value(), ui->pass->text());
    s.exec();

    //server.setModal(true); //exec();
    //this->on_join_clicked();
    //this->show();
    // delete this->server;
    // this->server = NULL;
}

void MainWindow::on_join_clicked()
{
    //QEventLoop *loop = new QEventLoop;

    //ClientUi *client = new ClientUi(this, ui->ip->text(), ui->port->value(), ui->name->text(), ui->pass->text());
    //client->show();
    ClientUi c(this, ui->ip->text(), ui->port->value(), ui->name->text(), ui->pass->text());
    c.exec();

    //ClientUi *bob = new ClientUi(this, ui->ip->text(), ui->port->value(), "bob", ui->pass->text());
    //ClientUi *timy = new ClientUi(this, ui->ip->text(), ui->port->value(), "timy", ui->pass->text());
    //ClientUi *rob = new ClientUi(this, ui->ip->text(), ui->port->value(), "rob", ui->pass->text());

    //connect(client, &Client::closed, loop, &QEventLoop::quit);
    //this->hide();
    //client->show();
    //bob->show();
    //timy->show();
    //rob->show();
    //loop->exec();
    //this->show();

}
