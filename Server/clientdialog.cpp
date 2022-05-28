#include "clientdialog.h"
#include "ui_ClientDialog.h"
#include <QImageReader>
#include <QPixmap>
#include <QImage>
#include <QMessageBox>

ClientDialog::ClientDialog(QTcpSocket* socket, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientDialog),
    socket(socket)

{
    ui->setupUi(this);

    stream.setDevice(socket);
    connect(socket, &QTcpSocket::readyRead, this, &ClientDialog::readFile);
}

ClientDialog::~ClientDialog()
{
    delete ui;
}

void ClientDialog::readFile()
{
    stream.setVersion(QDataStream::Qt_6_2);
    stream.startTransaction();
    QByteArray data;
    stream >> data;

    if (!stream.commitTransaction())
        return;

    showImage(data);

    socket->connect(socket, &QTcpSocket::disconnected, socket, &QObject::deleteLater);
    socket->disconnectFromHost();
}

void ClientDialog::showImage(const QByteArray& data)
{
  auto image = QImage::fromData(data);

    if (image.isNull())
    {
        QMessageBox::warning(this, "Warning", "Format invalied");
            return;
    }

    auto pmap = QPixmap::fromImage(image);

    ui->image->setPixmap(pmap);
    ui->image->setScaledContents(true);

    show();
    exec();

}
