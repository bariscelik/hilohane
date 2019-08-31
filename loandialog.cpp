#include "loandialog.h"
#include "ui_loandialog.h"
#include <QAbstractItemView>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>

LoanDialog::LoanDialog(QWidget *parent, int sc_id) :
    QDialog(parent),
    ui(new Ui::LoanDialog),
    schoolID(sc_id)
{
    ui->setupUi(this);

    this->setWindowTitle("Ödünç Ver");

    loadValues();

    ui->comboBox_student->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->comboBox_book->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    ui->dateTimeEdit_deliveryDate->setDateTime(QDateTime::currentDateTime());
    ui->dateTimeEdit_maxReturnDate->setDateTime(QDateTime::currentDateTime().addDays(7));


}

LoanDialog::LoanDialog(QWidget *parent, int l, int sc_id) :
    QDialog(parent),
    ui(new Ui::LoanDialog),
    loanID(l),
    schoolID(sc_id)
{
    ui->setupUi(this);

    this->setWindowTitle("Düzenle");

    QSqlQuery lq;
    lq.prepare("SELECT book_id, student_id, delivery_date, max_return_date FROM loans WHERE loanID = :lid LIMIT 1");
    lq.bindValue(":lid", loanID);
    lq.exec();

    if (lq.lastError().isValid())
        QMessageBox::critical(0,"Error",lq.lastError().text() );
    else
        lq.first();


   loadValues();

    ui->comboBox_student->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->comboBox_book->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // *********************** //
    // **** SET FORM DATA **** //
    // *********************** //
    ui->dateTimeEdit_deliveryDate->setDateTime(QDateTime::fromSecsSinceEpoch(lq.value(2).toLongLong()));
    ui->dateTimeEdit_maxReturnDate->setDateTime(QDateTime::fromSecsSinceEpoch(lq.value(3).toLongLong()));

    int i = ui->comboBox_student->findData(lq.value(1).toInt());
    if ( i != -1 )
        ui->comboBox_student->setCurrentIndex(i);

    int j = ui->comboBox_book->findData(lq.value(0).toInt());
    if ( j != -1 )
        ui->comboBox_book->setCurrentIndex(j);


}

LoanDialog::~LoanDialog()
{
    delete ui;
}

void LoanDialog::loadValues()
{

    QSqlQuery bq;
    bq.prepare("SELECT bookID, title, author FROM books WHERE school_id = :sc_id");
    bq.bindValue(":sc_id", schoolID);
    bq.exec();

    QSqlQuery sq;
    sq.prepare("SELECT id, name, class FROM students WHERE school_id = :sc_id");
    sq.bindValue(":sc_id", schoolID);
    sq.exec();

    if (bq.lastError().isValid())
        QMessageBox::critical(0,"Error",bq.lastError().text() );

    while(bq.next())
        ui->comboBox_book->addItem(bq.value(1).toString() + QString(" | ") + bq.value(2).toString(),bq.value(0).toInt());

    if (sq.lastError().isValid())
        QMessageBox::critical(0,"Error",sq.lastError().text() );

    while(sq.next())
        ui->comboBox_student->addItem(sq.value(1).toString() + QString(" | ") + sq.value(2).toString(),sq.value(0).toInt());

}

void LoanDialog::on_buttonBox_accepted()
{
    QSqlQuery item;

    if(loanID == 0)
    {
        item.prepare("INSERT INTO loans (book_id, delivery_date, max_return_date, student_id)"
                     "VALUES (:bid, :dd, :mrd, :sid)");
        item.bindValue(":bid", ui->comboBox_book->currentData().toInt());
        item.bindValue(":dd", ui->dateTimeEdit_deliveryDate->dateTime().toSecsSinceEpoch());
        item.bindValue(":mrd", ui->dateTimeEdit_maxReturnDate->dateTime().toSecsSinceEpoch());
        item.bindValue(":sid", ui->comboBox_student->currentData().toInt());

    }else{
        item.prepare("UPDATE loans SET book_id = :bid, delivery_date = :dd, max_return_date = :mrd, student_id = :sid WHERE loanID = :lid");
        item.bindValue(":lid", loanID);
        item.bindValue(":bid", ui->comboBox_book->currentData().toInt());
        item.bindValue(":dd", ui->dateTimeEdit_deliveryDate->dateTime().toSecsSinceEpoch());
        item.bindValue(":mrd", ui->dateTimeEdit_maxReturnDate->dateTime().toSecsSinceEpoch());
        item.bindValue(":sid", ui->comboBox_student->currentData().toInt());
    }

    item.exec();

    if (item.lastError().isValid())
        QMessageBox::critical(0,"Error",item.lastError().text() );
}
