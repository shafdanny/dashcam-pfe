QT += core network gui widgets multimediawidgets

CONFIG += c++11

TARGET = Client
CONFIG += console
#CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    imagedownloader.cpp \
    imagedecrypter.cpp \
    imagereader.cpp \
    trivium.cpp \
    tcpsocket.cpp \
    mainwindow.cpp \
    tcpsocketstreaming.cpp \
    tcpserverstreaming.cpp \
    settingsdialog.cpp \
    settings.cpp

HEADERS += \
    imagedownloader.h \
    imagedecrypter.h \
    imagereader.h \
    trivium.h \
    tcpsocket.h \
    mainwindow.h \
    tcpsocketstreaming.h \
    tcpserverstreaming.h \
    settingsdialog.h \
    settings.h

FORMS += \
    mainwindow.ui \
    settingsdialog.ui
