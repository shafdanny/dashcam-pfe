#ifndef IMAGEDECRYPTER_H
#define IMAGEDECRYPTER_H

#include <QObject>
#include <QFile>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <qDebug>


class ImageDecrypter : public QObject
{
    Q_OBJECT
    QByteArray decryptedData;
public:
    explicit ImageDecrypter(QByteArray cryptedData, QObject *parent = 0);
    QByteArray getDecryptedData();

signals:

public slots:
};

#endif // IMAGEDECRYPTER_H
