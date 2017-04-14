#ifndef TCPSOCKETSTREAMING_H
#define TCPSOCKETSTREAMING_H

#include <QObject>
#include <QTcpSocket>
#include "settings.h"

class TcpSocketStreaming : public QObject
{
    Q_OBJECT
    QTcpSocket* socket;
public:
    explicit TcpSocketStreaming(QObject *parent = 0);
    void connect();
    void disconnect();

signals:
    void socketDisconnected();
    void dataReceived(QByteArray data);

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();
};

#endif // TCPSOCKETSTREAMING_H
