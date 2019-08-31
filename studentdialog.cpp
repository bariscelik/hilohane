#include "studentdialog.h"
#include "ui_studentdialog.h"
#include <QMessageBox>
#include <QPushButton>
#include <QSqlQuery>
#include <QSqlError>

StudentDialog::StudentDialog(QWidget *parent, int sc_id) :
    QDialog(parent),
    ui(new Ui::StudentDialog),
    schoolID(sc_id)
{
    ui->setupUi(this);

    this->setWindowTitle("Öğrenci Ekle");

    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("Vazgeç");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Ekle");

}

StudentDialog::StudentDialog(QWidget *parent, StudentForm &f, int sc_id) :
    QDialog(parent),
    ui(new Ui::StudentDialog),
    studentID(f.studentID),
    schoolID(sc_id)
{
    ui->setupUi(this);

    this->setWindowTitle("Öğrenciyi Düzenle");

    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("Vazgeç");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Kaydet");

    // *********************** //
    // **** SET FORM DATA **** //
    // *********************** //
    ui->lineEdit_name->setText(f.name);
    ui->spinBox_number->setValue(f.number);
    ui->lineEdit_class->setText(f.cl);
}

StudentDialog::~StudentDialog()
{
    delete ui;
}

void StudentDialog::on_buttonBox_accepted()
{
    QSqlQuery item;

    if(studentID == 0)
    {
        item.prepare("INSERT INTO students (name, number, class, school_id)"
                     "VALUES (:name, :number, :class, :sc_id)");
        item.bindValue(":name", ui->lineEdit_name->text().trimmed());
        item.bindValue(":number", ui->spinBox_number->value());
        item.bindValue(":class", ui->lineEdit_class->text());
        item.bindValue(":sc_id", schoolID);

    }else{
        item.prepare("UPDATE students SET name=:name, number=:number, class=:class WHERE id=:id");
        item.bindValue(":id", studentID);
        item.bindValue(":name", ui->lineEdit_name->text().trimmed());
        item.bindValue(":number", ui->spinBox_number->value());
        item.bindValue(":class", ui->lineEdit_class->text());
    }

    item.exec();

    if (item.lastError().isValid())
        QMessageBox::critical(0,"Error",item.lastError().text() );
}
