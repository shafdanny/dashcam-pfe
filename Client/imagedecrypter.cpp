#include "imagedecrypter.h"

ImageDecrypter::ImageDecrypter(QByteArray cryptedData, QObject *parent) : QObject(parent)
{
    //saveKey("test_imake.key", cryptedData.size());
   /* Trivium trivium("test_imake.key", cryptedData.size());

    QByteArray keyData(trivium.getByteKey(), cryptedData.size());
    unsigned char outbyte;
    for (int i = 0; i < cryptedData.size(); i++)
    {
        outbyte = cryptedData.at(i) ^ keyData[i];
        decryptedData.append(outbyte);
    }*/
    //On décrypte avec un XOR sur la clé et les données

}

QByteArray ImageDecrypter::getDecryptedData()
{
    return decryptedData;
}
