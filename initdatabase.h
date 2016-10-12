#ifndef INITDATABASE_H
#define INITDATABASE_H

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QDir>

//! [0]
static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(QDir::currentPath() + "/hilohane.db");
    if (!db.open()) {
        QMessageBox::critical(0, "Veritabanı Hatası","Bağlantı sağlanamadı !", QMessageBox::Cancel);
        return false;
    }
    return true;
}
//! [0]

#endif // INITDATABASE_H
