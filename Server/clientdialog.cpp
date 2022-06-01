#include "clientdialog.h"
#include "ui_ClientDialog.h"
#include <QPixmap>
#include <QImage>
#include <QMessageBox>

ClientDialog::ClientDialog(QTcpSocket* socket, QWidget *parent) :
    QDialog(parent),
    socket(socket),
    ui(new Ui::ClientDialog)

{
    ui->setupUi(this);

    stream.setVersion(QDataStream::Qt_6_2);
    stream.setDevice(socket);

    connect(socket, &QTcpSocket::readyRead, this, &ClientDialog::readFile);
}

ClientDialog::~ClientDialog()
{
    delete ui;
}

void ClientDialog::readFile()
{
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
        QMessageBox::warning(this, "Warning", "Format is invalid");
        return;
    }

    auto pmap = QPixmap::fromImage(image);
    //TODO: Implement ability to scale window and content for saveing image aspect ratio.
    ui->image->setPixmap(pmap);
    ui->image->setScaledContents(true);

    show();
    exec();
}
