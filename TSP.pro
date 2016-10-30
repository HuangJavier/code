#-------------------------------------------------
#
# Project created by QtCreator 2016-10-27T15:51:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CXXFLAGS += -std=c++11

TARGET = TSP
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    tsp.cpp \
    greedy.cpp \
    backfire.cpp

HEADERS  += mainwindow.h \
    tsp.h \
    greedy.h \
    backfire.h

FORMS    += mainwindow.ui
