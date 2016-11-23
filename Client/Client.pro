QT += core network
QT -= gui

CONFIG += c++11

TARGET = Client
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    imagedownloader.cpp \
    imagedecrypter.cpp \
    imagereader.cpp

HEADERS += \
    imagedownloader.h \
    imagedecrypter.h \
    imagereader.h
