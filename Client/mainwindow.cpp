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
    dialog.setNameFilter(tr("Images (*.png *.jpg) *.bmp"));
    if (!dialog.exec())
        return;

    QStringList fileNames = dialog.selectedFiles();
    if (fileNames.isEmpty())
        return;
    ui->imagePath->setText(fileNames.first());
}

void MainWindow::on_sendfile_clicked()
{
    if (ui->serverAddr->text().isEmpty())
    {
        QMessageBox::warning(this, "Warning", "server address is empty");
            return;
    }
    if (ui->imagePath->text().isEmpty())
    {
        QMessageBox::warning(this, "Warning", "image path is empty");
            return;
    }

    auto addr = ui->serverAddr->text();
    socket->connectToHost(QHostAddress(addr), 55535);

}

void MainWindow::SendFileToServer()
{
    auto imgPath = ui->imagePath->text();

    QFile file(imgPath);
    file.open(QIODevice::ReadOnly);
    auto filebuf = file.readAll();

    QByteArray tosend;
    QDataStream stream(&tosend, QIODevice::WriteOnly);
    stream.setVersion(QDataStream::Qt_6_2);
    stream << filebuf;

    socket->write(tosend);
    socket->disconnectFromHost();
}

void MainWindow::ShowErrorMessage()
{
    QMessageBox::critical(this, "Error", "Can't connect to host, check host address");
}
