#include "initdatabase.h"
#define SQLITE_API extern
#define SQLITE_EXTERN
#include <sqlite3.h>
#include <QSqlDriver>
#include <QDebug>

initDatabase::initDatabase(){}

static void firstchar(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    qDebug("sadasdasdsadfdg");
    if (argc == 1) {
        const unsigned char *text = sqlite3_value_text(argv[0]);
        if (text && text[0]) {
          char result[2];
          result[0] = text[0]; result[1] = '\0';
          sqlite3_result_text(context, result, -1, SQLITE_TRANSIENT);
          return;
        }
    }
    sqlite3_result_null(context);
}

bool initDatabase::createConnection()
{

    sqlite3_initialize();
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::currentPath() + "/hilohane.db");

    if (!db.open()) {
        QMessageBox::critical(0, "Veritabanı Hatası","Bağlantı sağlanamadı !", QMessageBox::Cancel);
        return false;
    }

    return true;
}
