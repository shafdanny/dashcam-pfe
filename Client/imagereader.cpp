#include "imagereader.h"

ImageReader::ImageReader(QUrl url, QObject *parent) : QObject(parent), imageDownloader(url)
{
    connect(&imageDownloader, SIGNAL(downloaded()), this , SLOT(imageDownloaded()));
}

void ImageReader::imageDownloaded()
{
    qDebug() << "image downloaded";

    QByteArray bytesArray = imageDownloader.getImageData();
    qDebug() << "bytes : " << bytesArray.size();

    qDebug() << "decrypting data";
    ImageDecrypter imageDecrypter(bytesArray);
  //  connect(&imageDecrypter, SIGNAL(decrypted()), this, SLOT(imageDecrypted()));
    QByteArray decryptedData = imageDecrypter.getDecryptedData();

    qDebug() << "writing decrypted image to file";
    QFile imageFile("image.jpg");
    imageFile.open(QIODevice::WriteOnly);
    imageFile.write(decryptedData);
    imageFile.close();

}
