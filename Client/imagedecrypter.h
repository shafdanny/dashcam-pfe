#ifndef IMAGEDECRYPTER_H
#define IMAGEDECRYPTER_H

#include <QObject>
#include <QFile>

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
