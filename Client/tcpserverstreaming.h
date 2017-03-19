#ifndef TCPSERVERSTREAMING_H
#define TCPSERVERSTREAMING_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include "settings.h"
#include "trivium.h"

class TcpServerStreaming : public QObject
{
    Q_OBJECT
    QTcpServer* tcpServer;
    QTcpSocket *clientConnection;
    Trivium* t;
    QByteArray buffer;
    int cpt = 0;
public:
    explicit TcpServerStreaming(Trivium* t, QObject *parent = 0);
    void start();

signals:

public slots:
    void clientConnect();
    void sendData(QByteArray byteArray);
};

#endif // TCPSERVERSTREAMING_H
