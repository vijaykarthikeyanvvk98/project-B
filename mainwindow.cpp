#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    socket = new QUdpSocket(this);
    socket->setSocketOption(QAbstractSocket::ReceiveBufferSizeSocketOption, 1024*1024); // 1MB buffer
    connect(socket,&QUdpSocket::readyRead, this, &MainWindow::processResponse);

}

MainWindow::~MainWindow()
{
    socket->close();
}

void MainWindow::processResponse()
{
    if (!socket) {
        qDebug() << "socket closed";
        return;
    }
    if (socket->state() == QUdpSocket::BoundState) {

        if (socket->hasPendingDatagrams()) {


            datagram.clear();

            datagram.resize(socket->pendingDatagramSize());
            socket->readDatagram(datagram.data(),
                                                datagram.size(),
                                                &senderAddress,
                                                &senderPort);
            if (slen == -1) {
                // break;
            }
            databuffer.append(datagram);

            QString data = datagram.toHex();

            if (databuffer.size() > 10 * 1024) {
                emit data_received(databuffer);
                //qDebug()<<"Greater than 10*1024";
                databuffer.clear();
                //continue;
            }
            else
                emit data_received(databuffer);
            //qDebug()<<"Less than greater than 10*1024";
            databuffer.clear();
            //qDebug() << databuffer;
        }
        //-- Send whatever is left
        if (databuffer.size()) {
            emit data_received(databuffer);
            //qDebug()<<"Size"<<databuffer.size();
            databuffer.clear();
        }
    }
}
