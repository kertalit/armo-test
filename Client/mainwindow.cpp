#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qmessagebox.h"
#include "qfiledialog.h"
#include <QDataStream>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , socket ( new QTcpSocket(this) )
{
    ui->setupUi(this);

    connect(socket, &QTcpSocket::connected, this, &MainWindow::SendFileToServer);
    connect(socket, &QTcpSocket::errorOccurred, this, &MainWindow::ShowErrorMessage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openimage_clicked()
{
    QFileDialog dialog(this);
    dialog.setNameFilter("Images (*.png *.jpg *.bmp)");
    if (!dialog.exec())
        return;

    auto fileNames = dialog.selectedFiles();
    if (fileNames.isEmpty())
        return;

    ui->imagePath->setText(fileNames.first());
}

void MainWindow::on_sendfile_clicked()
{
    auto address = QHostAddress(ui->serverAddr->text());

    if (address.isNull())
    {
        QMessageBox::warning(this, "Warning", "Invalid server address");
        return;
    }
    if (ui->imagePath->text().isEmpty())
    {
        QMessageBox::warning(this, "Warning", "Image path is empty");
        return;
    }

    socket->connectToHost(address, PORT);
}

void MainWindow::SendFileToServer()
{
    QFile file(ui->imagePath->text());

    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, "Warning", "File is not open");
        return;
    }

    QByteArray tosend;
    QDataStream stream(&tosend, QIODevice::WriteOnly);

    stream.setVersion(QDataStream::Qt_6_2);
    stream << file.readAll();

    socket->write(tosend);
    socket->disconnectFromHost();
}

void MainWindow::ShowErrorMessage()
{
    QMessageBox::critical(this, "Error", "Can't connect to host, check host address");
}
