#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void processResponse();
private:
    QUdpSocket *socket=nullptr;
    QHostAddress receiverAddress=QHostAddress("192.168.56.3");
    quint16 receiverPort = 10050;
    QByteArray datagram;
    QByteArray UDP_DATA;
    QHostAddress senderAddress;
    quint16 senderPort;
    QByteArray databuffer;
    qint64 slen;

signals:
    void data_received(QByteArray);
};
#endif // MAINWINDOW_H
