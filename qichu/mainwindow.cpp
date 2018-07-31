#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->client = NULL;
    this->server = NULL;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_create_clicked()
{
    if (this->server == NULL)
        this->server = new Server(this, ui->port->value(), ui->pass->text());
    //    this->hide();
    this->server->show(); //exec();
    //    this->show();
    // delete this->server;
    // this->server = NULL;
}

void MainWindow::on_join_clicked()
{
    QEventLoop loop;
    if (this->client == NULL)
        this->client = new Client(this, ui->ip->text(), ui->port->value(), ui->name->text(), ui->pass->text());
    this->hide();
    connect(this->client, &Client::finished, &loop, &QEventLoop::quit);
    this->client->show();
    loop.exec();
    this->show();
    disconnect(this->client, &Client::finished, &loop, &QEventLoop::quit);
    delete this->client;
    this->client = NULL;
}
