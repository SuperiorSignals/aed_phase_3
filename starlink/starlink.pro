#-------------------------------------------------
#
# Project created by QtCreator 2018-02-23T12:08:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = starlink
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    entrydialog.cpp \
    adminlogindialog.cpp \
    checkform.cpp \
    checklist.cpp \
    user.cpp \
    checkdialog.cpp \
    userlogindialog.cpp \
    adminmain.cpp \
    usermain.cpp \
    database.cpp \
    userdata.cpp \
    reportdialog.cpp

HEADERS  += mainwindow.h \
    entrydialog.h \
    adminlogindialog.h \
    checkform.h \
    checklist.h \
    user.h \
    checkdialog.h \
    userlogindialog.h \
    adminmain.h \
    usermain.h \
    database.h \
    userdata.h \
    debuginfo.h \
    reportdialog.h

FORMS    += mainwindow.ui \
    entrydialog.ui \
    adminlogindialog.ui \
    checkform.ui \
    checkdialog.ui \
    userlogindialog.ui \
    adminmain.ui \
    usermain.ui \
    reportdialog.ui
