#include "serverwindow.h"
#include "./ui_serverwindow.h"
#include <QTcpSocket>
#include <QMessageBox>
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

    for (const auto& ipAddress : ipAddressesList)
    {
        if (ipAddress != QHostAddress::LocalHost && ipAddress.toIPv4Address()) // excpect localhost here, because it`s important to append it last
        {
            ui->netIntefaces->addItem(ipAddress.toString());
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
        QMessageBox::critical(this, "Error", "No servers available");
        return;
    }

    if(!server->listen(address, ServerWindow::PORT))
    {
        QMessageBox::critical(this, "Error", "Server is not started");
        return;
    }

    connect(server, &QTcpServer::newConnection, this, &ServerWindow::acceptConnection);

    QString port;
    ui->logServer->setText("The server started on IP: " + address.toString() + " Port: " + port.setNum(PORT));
}

void ServerWindow::on_stopServer_clicked()
{
    ui->startServer->setEnabled(true);
    ui->stopServer->setEnabled(false);

    server->close();

    ui->logServer->setText("The server has stopped");
}

