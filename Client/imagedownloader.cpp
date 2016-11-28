#include "imagedownloader.h"

ImageDownloader::ImageDownloader(QUrl url, QObject *parent) : QObject(parent)
{
    qDebug() << "Donwloading image ...";
    connect(&networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(imageDownloaded(QNetworkReply*)));
    QNetworkRequest request(url);
    networkManager.get(request);
}

QByteArray ImageDownloader::getImageData()
{
    return imageData;
}

void ImageDownloader::imageDownloaded(QNetworkReply *reply)
{
    imageData = reply->readAll();
    reply->deleteLater();
    emit downloaded();
}
