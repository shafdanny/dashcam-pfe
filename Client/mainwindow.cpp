#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    settingsDialog = new SettingsDialog();


    mediaPlayer = new QMediaPlayer(this, QMediaPlayer::StreamPlayback);
    mediaPlayer->setVideoOutput(ui->imageWidget);
    mediaPlaylist = new QMediaPlaylist(mediaPlayer);

    //buffer = new QBuffer();
    //buffer->open(QBuffer::ReadWrite);

   /* tcpSocketImages = new TcpSocket();
    QObject::connect(tcpSocketImages, SIGNAL(imageReceived(QByteArray)), this, SLOT(displayImage(QByteArray)));
    tcpSocketImages->connect();*/

    t.initiation();
    tcpServerStreaming = new TcpServerStreaming(&t);
    tcpSocketStreaming = new TcpSocketStreaming();
    QObject::connect(tcpSocketStreaming, SIGNAL(socketDisconnected()), this, SLOT(disconnected()));

    //On connecte la reception de donnees par la socket cliente à l'envoi des données de la socket serveur
    QObject::connect(tcpSocketStreaming, SIGNAL(dataReceived(QByteArray)), tcpServerStreaming, SLOT(sendData(QByteArray)));



  //  displayImage(QByteArray());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::displayImage(QByteArray byteArray)
{
    qDebug() << "display image";

    QFile file( QString::number(QDateTime().currentMSecsSinceEpoch()) + "big.jpg");
    file.open(QFile::ReadWrite);
    file.write(byteArray);

    QPixmap pix;
    pix.loadFromData(byteArray, "JPG");
    ui->label->setPixmap(pix);
  /*  mediaPlayer->setMedia(NULL, bufferImage);
    ui->imageWidget->show();
    mediaPlayer->play();*/
}

void MainWindow::showSettings()
{
    settingsDialog->exec();
    Settings::getInstance()->saveSettings();
}

void MainWindow::startStreaming()
{
    if (ui->pushButtonConnect->text() == "Connexion")
    {
        tcpSocketStreaming->connect();
        tcpServerStreaming->start();
        ui->pushButtonConnect->setText("Deconnexion");
    }
    else
    {
        tcpSocketStreaming->disconnect();
        ui->pushButtonConnect->setText("Connexion");
    }
}

void MainWindow::disconnected()
{
    ui->pushButtonConnect->setText("Connexion");
}
