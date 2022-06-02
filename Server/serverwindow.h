#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include "clientdialog.h"
#include <QTcpServer>

QT_BEGIN_NAMESPACE
namespace Ui { class ServerWindow; }
QT_END_NAMESPACE

class ServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    ServerWindow(QWidget *parent = nullptr);
    ~ServerWindow();

private slots:
    void acceptConnection();
    void on_startServer_clicked();
    void on_stopServer_clicked();

private:
    Ui::ServerWindow *ui;
    QTcpServer* server;

    static const quint16 PORT = 55535;
};
#endif // SERVERWINDOW_H
