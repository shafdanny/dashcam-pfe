#include <QApplication>
#include <QUrl>
#include "imagereader.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
  //  QUrl url("http://192.168.1.132/images/test_image_trivium_crypted.jpg");
  //  QUrl url(" C:\\Users\\user\\Documents\\pfe\\dashcam-pfe\\gpu_programming\\Tests\\test_image_crypted.jpg");
  //  ImageReader imageReader(url);
    return a.exec();
}
