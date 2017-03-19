#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include "settings.h"
#include "trivium.h"

class TcpSocket : public QObject
{
    Q_OBJECT
    QTcpSocket* socket;
    QByteArray image;
    Trivium t;
    char dataImage[250000];
    int sizeImageReceived, imageSize;
public:
    explicit TcpSocket(QObject *parent = 0);
    void connect();

signals:
    void imageReceived(QByteArray byteArray);

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();
};

#endif // TCPSOCKET_H
