#ifndef IMAGEREADER_H
#define IMAGEREADER_H

#include <QObject>
#include <QFile>
#include "imagedownloader.h"
#include "imagedecrypter.h"

class ImageReader : public QObject
{
    Q_OBJECT
    ImageDownloader imageDownloader;
public:
    explicit ImageReader(QUrl url, QObject *parent = 0);

signals:

public slots:
    void imageDownloaded();
};

#endif // IMAGEREADER_H
