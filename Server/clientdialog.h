#ifndef CLIENTDIALOG_H
#define CLIENTDIALOG_H

#include <QDialog>
#include "qtcpsocket.h"


namespace Ui {
class ClientDialog;
}

class ClientDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClientDialog(QTcpSocket* socket, QWidget *parent = nullptr);
    ~ClientDialog();

    void showImage(const QByteArray& data);

public slots:
    void readFile();

private:
    QDataStream stream;
    QTcpSocket* socket;

    Ui::ClientDialog *ui;
};

#endif // CLIENTDIALOG_H
