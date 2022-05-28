#include "serverwindow.h"
#include "./ui_serverwindow.h"
#include <QTcpSocket>
#include <QFile>
#include <QPixmap>
#include <QNetworkInterface>

ServerWindow::ServerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ServerWindow)
{
    ui->setupUi(this);

    server = new QTcpServer(this);

    auto ipAddressesList = QNetworkInterface::allAddresses();

    for (int i = 0; i < ipAddressesList.size(); ++i) // range based for check
    {
        auto current = ipAddressesList.at(i);

        if (current != QHostAddress::LocalHost && current.toIPv4Address()) // comment here
        {
            ui->netIntefaces->addItem(current.toString());
        }
    }

    ui->netIntefaces->addItem(QHostAddress(QHostAddress::LocalHost).toString());
    ui->netIntefaces->setCurrentIndex(ui->netIntefaces->count() - 1);

    ui->stopServer->setEnabled(false);


}

ServerWindow::~ServerWindow()
{
    for (auto c : clients)
        delete c;

    delete ui;
}

void ServerWindow::acceptConnection()
{
    auto socket = server->nextPendingConnection();
    clients.push_back(new ClientDialog(socket, this));
}


void ServerWindow::on_startServer_clicked()
{
    ui->stopServer->setEnabled(true);
    ui->startServer->setEnabled(false);

    auto address =  QHostAddress(ui->netIntefaces->currentText());

    if (address.isNull())
    {
        //error
        return;
    }

    if(!server->listen(address, 55535)) // port static const "PORT", if listen unvallid, if QHostAd
    {
        //error
        return;
    }

    connect(server, &QTcpServer::newConnection, this, &ServerWindow::acceptConnection);
    ui->logServer->setText("The server started on IP: " + address.toString()); //server->serverAddress().toString()); //check!! add port
}


void ServerWindow::on_stopServer_clicked()
{
    ui->startServer->setEnabled(true);
    ui->stopServer->setEnabled(false);

    server->close();

    ui->logServer->setText("The server has stopped");  //check!!
}

