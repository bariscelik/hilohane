#-------------------------------------------------
#
# Project created by QtCreator 2016-09-23T20:19:08
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HiloHane
TEMPLATE = app
CONFIG += c++11

install_db.path = %{buildDir}
install_db.files += %{sourceDir}/hilohane.db

INSTALLS += install_db


include(/home/baris/Projeler/qtLibraries/QtXlsxWriter/src/xlsx/qtxlsx.pri)

SOURCES += main.cpp\
        home.cpp \
    addrecord.cpp \
    students.cpp

HEADERS  += home.h \
    initdatabase.h \
    addrecord.h \
    students.h

FORMS    += home.ui \
    addrecord.ui \
    students.ui \
    addstudent.ui \
    recordsbystudent.ui

DISTFILES += \
    hilohane.db

RESOURCES += \
    hilohane.qrc
