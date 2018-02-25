QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HiloHane
TEMPLATE = app
CONFIG += c++11

install_db.files = hilohane.db
install_db.path  = $$OUT_PWD

INSTALLS += install_db


win32{
    include(C:/qt_projects/qtLibraries/QtXlsxWriter/src/xlsx/qtxlsx.pri)
} else {
    include(/home/baris/Projeler/qtLibraries/QtXlsxWriter/src/xlsx/qtxlsx.pri)
}


SOURCES +=  main.cpp\
            home.cpp \
            addrecord.cpp \
            students.cpp \
            initdatabase.cpp \
            editrecord.cpp \
            multiplefiltermodel.cpp \
    tools.cpp \
    returnform.cpp

HEADERS  += home.h \
            initdatabase.h \
            addrecord.h \
            students.h \
            editrecord.h \
            hiloheaderview.h \
            multiplefiltermodel.h \
    tools.h \
    coloredsqlquerymodel.h \
    returnform.h

FORMS    += home.ui \
    addrecord.ui \
    editrecord.ui \
    students.ui \
    addstudent.ui \
    recordsbystudent.ui \
    tools.ui \
    returnform.ui

DISTFILES += \
    hilohane.db

RESOURCES += \
    hilohane.qrc







