#ifndef INITDATABASE_H
#define INITDATABASE_H

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QDir>

class initDatabase{
public:
    initDatabase();
    bool createConnection();
    QSqlDatabase db;
};

#endif // INITDATABASE_H
