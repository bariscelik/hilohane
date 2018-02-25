#include "tools.h"
#include "ui_tools.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QDateTime>

tools::tools(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::tools)
{
    ui->setupUi(this);
}

tools::~tools()
{
    delete ui;
}

void tools::on_deleteAllBookDataBtn_clicked()
{
    QMessageBox m(QMessageBox::Question, "Silme Onayı", "!!! TÜM KİTAP ALMA VE İADE KAYITLARI SİLİNECEKTİR. EMİN MİSİNİZ ? \n (VERİTABANI YEDEĞİ ALMAYI UNUTMAYIN)" , QMessageBox::Yes|QMessageBox::No);
    m.setButtonText(QMessageBox::Yes, tr("Evet"));
    m.setButtonText(QMessageBox::No, tr("Hayır"));

    if(QMessageBox::Yes == m.exec())
    {
        QSqlQuery("DELETE FROM books");
        QSqlQuery("VACUUM");
    }

    emit updateModel();
}

void tools::on_increaseClassNumberBtn_clicked()
{
    QMessageBox m(QMessageBox::Question, "Emin misiniz?", "Öğrencilerin sınıfları belirttiğiniz miktarda artırılacak ya da azaltılacaktır. Emin misiniz ? \n (VERİTABANI YEDEĞİ ALMAYI UNUTMAYIN)" , QMessageBox::Yes|QMessageBox::No);
    m.setButtonText(QMessageBox::Yes, tr("Evet"));
    m.setButtonText(QMessageBox::No, tr("Hayır"));

    if(QMessageBox::Yes == m.exec())
    {
        if(ui->incrementNumSpbx->value()!= 0)
        {
            QSqlQuery query("SELECT id,class FROM students");

            if (query.lastError().isValid()) QMessageBox::critical(0,"Hata",query.lastError().text() );


            QString className = query.record().value("class").toString();
            int classNum=0;
            int newClassNum=0;

            while(query.next())
            {
                className = query.record().value("class").toString();
                int classNum = className.split("-")[0].toInt();
                int newClassNum = ui->incrementNumSpbx->value() + classNum;
                if(newClassNum>0)
                {
                    className.remove(0,1);
                    className = QString::number(ui->incrementNumSpbx->value() + classNum) + className;
                }

                QSqlQuery saveQuery;
                saveQuery.prepare("UPDATE students SET class = :className WHERE id = :id");
                saveQuery.bindValue(":id", query.record().value("id").toString());
                saveQuery.bindValue(":className", className);
                saveQuery.exec();

            }

        }

        emit updateModel();

    }

}


void tools::on_migrateDbBtn_clicked()
{
    std::map<int,std::map<QString,int> > data;

    QSqlQuery booksTable;
    booksTable.exec("SELECT id,delivery_date,max_return_date,return_date FROM books");

    while (booksTable.next()) {

        const int id = booksTable.value(0).toInt();

        data[id]["delivery_date"]   = QDateTime::fromString(booksTable.value(1).toString(), Qt::TextDate).toSecsSinceEpoch();
        data[id]["max_return_date"] = QDateTime::fromString(booksTable.value(2).toString(), Qt::TextDate).toSecsSinceEpoch();
        data[id]["return_date"]     = QDateTime::fromString(booksTable.value(3).toString(), Qt::TextDate).toSecsSinceEpoch();

    }

    for (const auto& kv : data) {
        QSqlQuery saveQuery;
        saveQuery.prepare("UPDATE books SET delivery_date = :dd, max_return_date = :mrd, return_date = :rd  WHERE id = :id");
        saveQuery.bindValue(":id", kv.first);
        saveQuery.bindValue(":dd", kv.second.at("delivery_date"));
        saveQuery.bindValue(":mrd", kv.second.at("max_return_date"));
        saveQuery.bindValue(":rd", kv.second.at("return_date"));
        saveQuery.exec();
    }
}
