#include "home.h"
#include <QApplication>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include "initdatabase.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    initDatabase *in = new initDatabase();
    in->createConnection();

    Home w;
    w.show();
    return a.exec();
}
