#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_openimage_clicked();
    void on_sendfile_clicked();

private:
    void SendFileToServer();
    void ShowErrorMessage(QTcpSocket::SocketError);

    Ui::MainWindow *ui;
    QTcpSocket* socket;
    static const qint32 PORT = 55535;

};
#endif // MAINWINDOW_H
