QT += core
QT -= gui
QT += network

CONFIG += c++11
CONFIG += console

TARGET = AutoLogin
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    networkcookie.cpp \
    autologin.cpp

HEADERS += \
    networkcookie.h \
    autologin.h
