#include "editrecord.h"
#include "ui_editrecord.h"

#include <QCompleter>
#include <QSqlQuery>
#include <QMessageBox>
#include "initdatabase.h"


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
    QStringList* list = new QStringList();
    QString bookTitle, pageNumber, deliveryDate, maxReturnDate;
    int studentId;

    QSqlQuery records;



    if(records.exec("SELECT book_title, page, delivery_date, max_return_date, student_id FROM books WHERE id=" +  QString::number(recordId) + " LIMIT 1"))
    {
        if(records.first())
        {
            bookTitle = records.value(0).toString();
            pageNumber = records.value(1).toString();
            deliveryDate = records.value(2).toString();
            maxReturnDate = records.value(3).toString();
            studentId = records.value(4).toInt();
        }

    }

    QSqlQuery books;

    if(books.exec("SELECT book_title FROM books GROUP BY book_title"))
    {
        while(books.next())
        {
            list->append(books.value(0).toString());
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

        QCompleter* completer = new QCompleter(*list);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        ui->bookLineEdit->setCompleter(completer);
        ui->bookLineEdit->setText(bookTitle);
        ui->sayfaSayisiLineEdit->setText(pageNumber);
        ui->deliveryDateEdit->setDateTime(QDateTime::fromString(deliveryDate));
        ui->sonIadeTarihiDateTimeEdit->setDateTime(QDateTime::fromString(maxReturnDate));
        ui->OgrenciComboBox->setCurrentIndex(ui->OgrenciComboBox->findData(studentId));
}

void editRecord::on_buttonBox_accepted()
{
    QSqlQuery item;

    item.prepare("UPDATE books SET book_title = :bt , delivery_date = :dd, max_return_date = :mrd, student_id = :si, page = :pg "
                 "WHERE id = :id");

    item.bindValue(":id", recordId);
    item.bindValue(":bt", ui->bookLineEdit->text());
    item.bindValue(":dd", ui->deliveryDateEdit->dateTime().toString());
    item.bindValue(":mrd", ui->sonIadeTarihiDateTimeEdit->dateTime().toString());
    item.bindValue(":si", ui->OgrenciComboBox->currentData().toInt());
    item.bindValue(":pg", ui->sayfaSayisiLineEdit->text().toInt());
    if(!item.exec())
    {
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
