#include "tools.h"
#include "ui_tools.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>

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

