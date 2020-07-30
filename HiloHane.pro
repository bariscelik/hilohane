QT       += core gui sql xml network printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HiloHane
TEMPLATE = app
CONFIG += c++11

install_db.files = hilohane.db
install_db.path  = $$OUT_PWD

INSTALLS += install_db

include($$PWD/QtXlsxWriter/src/xlsx/qtxlsx.pri)


SOURCES +=  main.cpp\
            home.cpp \
    loandialog.cpp \
            students.cpp \
            initdatabase.cpp \
            multiplefiltermodel.cpp \
    tools.cpp \
    returnform.cpp \
    books.cpp \
    bookdialog.cpp \
    studentdialog.cpp

HEADERS  += home.h \
    globals.h \
            initdatabase.h \
    loandialog.h \
            students.h \
            hiloheaderview.h \
            multiplefiltermodel.h \
    tools.h \
    coloredsqlquerymodel.h \
    returnform.h \
    books.h \
    bookdialog.h \
    studentdialog.h

FORMS    += home.ui \
    loandialog.ui \
    students.ui \
    recordsbystudent.ui \
    tools.ui \
    returnform.ui \
    books.ui \
    bookdialog.ui \
    studentdialog.ui

DISTFILES += \
    hilohane.db

RESOURCES += \
    hilohane.qrc
