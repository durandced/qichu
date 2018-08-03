#-------------------------------------------------
#
# Project created by QtCreator 2018-07-27T14:12:48
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qichu
TEMPLATE = app
CONFIG += c++14

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

CONFIG(debug, release|debug):DEFINES += _DEBUG
CONFIG(release, release|debug):DEFINES += _RELEASE

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES +=  main.cpp            \
            mainwindow.cpp      \
            server.cpp          \
            serverCallBack.cpp  \
            serverActions.cpp   \
            client.cpp          \
            clientCallBack.cpp  \
            clientActions.cpp   \
            card.cpp            \
            player.cpp          \
    	    board.cpp 		\
    	    core.cpp

HEADERS  += mainwindow.h        \
            server.h            \
            client.h            \
            card.h              \
            player.h            \
            core.h              \
            protocol.h          \
    	    board.h

FORMS    += mainwindow.ui       \
            server.ui           \
            client.ui           \
            player.ui           \
