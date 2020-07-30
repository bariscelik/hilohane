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


//UPDATE books SET fixture = fixture-3 WHERE school_id = 1 ORDER BY fixture ASC;

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

void tools::on_fixFixtureBtn_triggered(QAction *arg1)
{

}


void tools::on_fixFixtureBtn_clicked()
{


    QSqlQuery query("SELECT bookID FROM books WHERE school_id = 2 ORDER BY fixture ASC");

    if (query.lastError().isValid()) QMessageBox::critical(0,"Hata",query.lastError().text() );


    size_t f = 1;
    size_t bookID = 0;

    while(query.next())
    {
        bookID = query.record().value("bookID").toUInt();

        QSqlQuery saveQuery;
        saveQuery.prepare("UPDATE books SET fixture = :fix WHERE bookID = :id");
        saveQuery.bindValue(":id", QVariant::fromValue(bookID));
        saveQuery.bindValue(":fix", QVariant::fromValue(f));
        saveQuery.exec();
        f++;
    }


        emit updateModel();

}
