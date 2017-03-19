#include "tcpsocket.h"

TcpSocket::TcpSocket(QObject *parent) : QObject(parent)
{
    t.initiation();
}

void TcpSocket::connect()
{

    sizeImageReceived = 0;
    imageSize = 0;
    socket = new QTcpSocket(this);

    QObject::connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    QObject::connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    QObject::connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    QObject::connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

    socket->connectToHost(Settings::getInstance()->ipAddress, Settings::getInstance()->portClient);
    // tcp/h264://192.168.1.117:8000
    if(!socket->waitForConnected(5000))
    {
        qDebug() << "Error: " << socket->errorString();
    }
}

void TcpSocket::connected()
{
    qDebug() << "connected";
  //  socket->write("HEAD / HTTP/1.0\r\n\r\n\r\n\r\n");
}

void TcpSocket::disconnected()
{
    qDebug() << "disconnected";
}

void TcpSocket::bytesWritten(qint64 bytes)
{
    qDebug() << bytes << " bytes written...";
}

void TcpSocket::readyRead()
{
  //  QByteArray allData = socket->readAll();
  //  unsigned char* dataCh = t.cypher(reinterpret_cast<unsigned char*>(allData.data()), allData.size());
  //  QByteArray(reinterpret_cast<char*>(dataCh), 4));
    if (imageSize == 0)
    {
        char data[4];
        int size = socket->read(data, 4);

        unsigned char* dataCh = t.cypher(reinterpret_cast<unsigned char*>(data), 4);
        qDebug() << QString(QByteArray(reinterpret_cast<char*>(dataCh), 4));
/*        QByteArray sizeData(reinterpret_cast<char*>(dataCh), 4);
        QDataStream stream(sizeData);
        stream.setByteOrder(QDataStream::LittleEndian);
        stream >> imageSize;
        qDebug() << "image size : " << imageSize;*/
    }
    else
    {
        int read = socket->read(dataImage, imageSize);

        unsigned char* data = t.cypher(reinterpret_cast<unsigned char*>(dataImage), read);

        image.append(reinterpret_cast<char*>(data), read);
        imageSize -= read;
        if (imageSize == 0)
        {
            emit imageReceived(image);
            image.clear();
        }
    }
}

