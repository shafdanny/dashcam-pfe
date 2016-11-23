#ifndef IMAGEDECRYPTER_H
#define IMAGEDECRYPTER_H

#include <QObject>

class ImageDecrypter : public QObject
{
    Q_OBJECT
public:
    explicit ImageDecrypter(QObject *parent = 0);

signals:

public slots:
};

#endif // IMAGEDECRYPTER_H
