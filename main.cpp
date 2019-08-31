#include "home.h"
#include <QApplication>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include "initdatabase.h"
#include <QStyleFactory>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QInputDialog>
#include <QDebug>
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    initDatabase *in = new initDatabase();
    in->createConnection();

    a.setStyle(QStyleFactory::create("Fusion"));

    // *********************** //
    // **** SELECT SCHOOL **** //
    // *********************** //
    QStringList sl;
    QSqlQuery schools("SELECT schoolID, name FROM schools");

    while (schools.next())
        sl.append(schools.record().value(1).toString());

    bool ok;
    QInputDialog id;

    id.setFixedSize(id.width(),id.height());

    QString text = id.getItem(nullptr, "Okul Seçimi",
                                         "Okul:", sl,
                                         0, false, &ok);
    if(ok && !text.isEmpty())
    {
        int i = sl.indexOf(text);
        schools.seek(i);

        Home w(nullptr, schools.record().value(0).toInt());

        w.setWindowTitle(QString("HiloHane - ") + schools.record().value(1).toString());
        w.showMaximized();

        return a.exec();
    }
    else{
        return 0;
    }

}
