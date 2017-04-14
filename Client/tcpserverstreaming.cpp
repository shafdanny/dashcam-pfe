#include "tcpserverstreaming.h"

TcpServerStreaming::TcpServerStreaming(Trivium *t, QObject *parent) : QObject(parent)
{
    this->t = t;
    //On initialise la socket cliente
    clientConnection = new QTcpSocket();
    tcpServer = new QTcpServer(this);
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(clientConnect()));
}

void TcpServerStreaming::start()
{
    if (!tcpServer->listen(QHostAddress::Any, Settings::getInstance()->portServer))
    {
        qDebug() << "error listen";
    }
}

void TcpServerStreaming::clientConnect()
{
    clientConnection = tcpServer->nextPendingConnection();
    qDebug() << "client connecte";
}

void TcpServerStreaming::sendData(QByteArray byteArray)
{
    cpt += byteArray.size();
    buffer.append(byteArray);
    while (buffer.size() >= 2048)
    {
        QByteArray subByteArray = buffer.mid(0, 2048);
        buffer.remove(0, 2048);
        unsigned char* data = t->cypher((unsigned char*)subByteArray.data(), subByteArray.size());
        QByteArray dataDecrypted ((char*)data, subByteArray.size());
        if (clientConnection->isOpen())
        {
            qDebug() << "data size : " << dataDecrypted.size() << "-" << buffer.size() << "-" << cpt;
            clientConnection->write(dataDecrypted.constData(), dataDecrypted.size());
        }
    }
}
