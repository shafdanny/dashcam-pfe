#include "imagedecrypter.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <qDebug>


ImageDecrypter::ImageDecrypter(QByteArray cryptedData, QObject *parent) : QObject(parent)
{
    QFile keyFile("test_image.key");
    keyFile.open(QIODevice::ReadOnly);
    QByteArray keyData = keyFile.readAll();
    unsigned char outbyte;
    for (int i = 0; i < cryptedData.size(); i++)
    {
        outbyte = cryptedData.at(i) ^ keyData[i];
        decryptedData.append(outbyte);
    }
    //On décrypte avec un XOR sur la clé et les données

}

QByteArray ImageDecrypter::getDecryptedData()
{
    return decryptedData;
}
