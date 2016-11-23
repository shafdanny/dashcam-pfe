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
    QFile imageFile("image.jpg");
    imageFile.open(QIODevice::WriteOnly);
    imageFile.write(bytesArray);
    imageFile.close();

}
