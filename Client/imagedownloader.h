#ifndef IMAGEDOWNLOADER_H
#define IMAGEDOWNLOADER_H
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QObject>

class ImageDownloader : public QObject
{
    Q_OBJECT
    QNetworkAccessManager networkManager;
    QByteArray imageData;
public:
    explicit ImageDownloader(QUrl url, QObject *parent = 0);
    QByteArray getImageData();

signals:
    void downloaded();

public slots:
    void imageDownloaded(QNetworkReply* reply);
};

#endif // IMAGEDOWNLOADER_H
