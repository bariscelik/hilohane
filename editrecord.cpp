#include "editrecord.h"
#include "ui_editrecord.h"

#include <QCompleter>
#include <QSqlQuery>
#include <QMessageBox>
#include "initdatabase.h"
#include <QDebug>


editRecord::editRecord(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::editRecord)
{
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("Vazgeç");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Kaydet");
    ui->sayfaSayisiLineEdit->setValidator(new QIntValidator(0, 100000, this));

    connect(&st, SIGNAL(finished(int)), this, SLOT(updateModel()));
    connect(this, SIGNAL(finished(int)), this, SLOT(updateModel()), Qt::QueuedConnection);

    updateModel();
}

editRecord::~editRecord()
{
    delete ui;
}

void editRecord::updateModel()
{
    QStringList* bookList = new QStringList();
    QStringList *bookPageList = new QStringList();



    QString bookTitle, pageNumber;
    QDateTime deliveryDate,
            returnDate,
            maxReturnDate;
    int studentId;

    QSqlQuery records;

    if(records.exec("SELECT book_title, page, delivery_date, return_date, max_return_date, student_id FROM books WHERE id=" +  QString::number(recordId) + " LIMIT 1"))
    {
        if(records.first())
        {
            isActive = records.value(3).isNull();
            bookTitle = records.value(0).toString();
            pageNumber = records.value(1).toString();
            deliveryDate = QDateTime::fromSecsSinceEpoch(records.value(2).toLongLong());
            returnDate = QDateTime::fromSecsSinceEpoch(records.value(3).toLongLong());
            maxReturnDate = QDateTime::fromSecsSinceEpoch(records.value(4).toLongLong());
            studentId = records.value(5).toInt();
        }
    }

    QSqlQuery books;

    if(books.exec("SELECT book_title, page FROM books GROUP BY book_title"))
    {
        while(books.next())
        {
            bookList->append(books.value(0).toString());
            bookPageList->append(books.value(1).toString());
        }
    }

    QSqlQuery students;

    ui->OgrenciComboBox->clear();
    if(students.exec("SELECT id,name,number FROM students ORDER BY name"))
    {
        while(students.next())
        {
            ui->OgrenciComboBox->addItem(QIcon(),students.value(1).toString(),QVariant(students.value(0).toInt()));
        }
    }

    QCompleter* completer = new QCompleter(*bookList);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    connect(completer, QOverload<const QString &>::of(&QCompleter::activated),
        [=](const QString &s){ ui->sayfaSayisiLineEdit->setText(bookPageList->at(bookList->indexOf(s))); });

    ui->returnDateTimeEdit->setEnabled(!isActive);
    ui->returnDateLabel->setEnabled(!isActive);

    ui->bookLineEdit->setCompleter(completer);
    ui->bookLineEdit->setText(bookTitle);
    ui->sayfaSayisiLineEdit->setText(pageNumber);
    ui->deliveryDateEdit->setDateTime(deliveryDate);
    ui->returnDateTimeEdit->setDateTime(returnDate);
    ui->sonIadeTarihiDateTimeEdit->setDateTime(maxReturnDate);
    ui->OgrenciComboBox->setCurrentIndex(ui->OgrenciComboBox->findData(studentId));
}

void editRecord::on_buttonBox_accepted()
{
    QSqlQuery item;

    item.prepare("UPDATE books SET book_title = :bt , delivery_date = :dd, return_date = :rd, max_return_date = :mrd, student_id = :si, page = :pg "
                 "WHERE id = :id");

    item.bindValue(":id", recordId);
    item.bindValue(":bt", ui->bookLineEdit->text().trimmed());
    item.bindValue(":dd", QString::number(ui->deliveryDateEdit->dateTime().toSecsSinceEpoch()));
    if(!isActive)
        item.bindValue(":rd", QString::number(ui->returnDateTimeEdit->dateTime().toSecsSinceEpoch()));

    item.bindValue(":mrd",QString::number(ui->sonIadeTarihiDateTimeEdit->dateTime().toSecsSinceEpoch()));
    item.bindValue(":si", ui->OgrenciComboBox->currentData().toInt());
    item.bindValue(":pg", ui->sayfaSayisiLineEdit->text().toInt());
    if(!item.exec())
    {
        qDebug("de");
        QMessageBox msgbx(QMessageBox::Critical, "Kayıt Düzenleme", "Hata: " + item.lastQuery());
        msgbx.exec();
    }
}

void editRecord::on_pushButton_clicked()
{
    st.show();
}

void editRecord::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    updateModel();
}

void editRecord::setRecordId(int id)
{
    recordId = id;
}
