#include "initdatabase.h"

initDatabase::initDatabase(){}

bool initDatabase::createConnection()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::currentPath() + "/hilohane.db");
    if (!db.open()) {
        QMessageBox::critical(0, "Veritabanı Hatası","Bağlantı sağlanamadı !", QMessageBox::Cancel);
        return false;
    }
    return true;
}
