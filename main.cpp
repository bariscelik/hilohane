#include "home.h"
#include <QApplication>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include "initdatabase.h"
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    initDatabase *in = new initDatabase();
    in->createConnection();

    a.setStyle(QStyleFactory::create("Fusion"));

    Home w;
    w.showMaximized();

    return a.exec();
}
