#include <QCoreApplication>
#include <QUrl>
#include "imagereader.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QUrl url("http://192.168.1.132/images/image_encrypted.jpg");
    ImageReader imageReader(url);
    return a.exec();
}
