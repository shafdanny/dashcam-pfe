#include "tcpsocketstreaming.h"

TcpSocketStreaming::TcpSocketStreaming(QObject *parent) : QObject(parent)
{

}

void TcpSocketStreaming::connect()
{
    socket = new QTcpSocket(this);

    QObject::connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    QObject::connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    QObject::connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    QObject::connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

    socket->connectToHost(Settings::getInstance()->ipAddress, Settings::getInstance()->portServer);
    // tcp/h264://192.168.1.117:8000
    if(!socket->waitForConnected(45000))
    {
        qDebug() << "Error: " << socket->errorString();
    }
}

 void TcpSocketStreaming::disconnect()
 {
     socket->disconnectFromHost();
 }

void TcpSocketStreaming::connected()
{
    qDebug() << "connected";
}

void TcpSocketStreaming::disconnected()
{
    qDebug() << "disconnected";
    emit socketDisconnected();
}

void TcpSocketStreaming::bytesWritten(qint64 bytes)
{
    qDebug() << bytes << " bytes written...";
}

void TcpSocketStreaming::readyRead()
{
    emit dataReceived(socket->readAll());
}


