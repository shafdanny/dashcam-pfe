#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVideoWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QBuffer>
#include <QDateTime>
#include <QFile>
#include "tcpsocket.h"
#include "tcpsocketstreaming.h"
#include "tcpserverstreaming.h"

#include "trivium.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include "settingsdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
  //  QBuffer* buffer;
    Trivium t;
    TcpSocket* tcpSocketImages;
    TcpSocketStreaming* tcpSocketStreaming;
    TcpServerStreaming* tcpServerStreaming;
    SettingsDialog* settingsDialog;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void displayImage(QByteArray byteArray);
    void showSettings();
    void startStreaming();
    void disconnected();

private:
    Ui::MainWindow *ui;
    QMediaPlayer* mediaPlayer;
    QMediaPlaylist* mediaPlaylist;
};

#endif // MAINWINDOW_H
