#include "addrecord.h"
#include "ui_addrecord.h"

#include <QCompleter>
#include <QSqlQuery>
#include <QMessageBox>
#include "initdatabase.h"
#include <QDebug>

addRecord::addRecord(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addRecord)
{
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("Vazgeç");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Ekle");
    ui->sayfaSayisiLineEdit->setValidator(new QIntValidator(0, 100000, this));


    connect(&st, SIGNAL(finished(int)), this, SLOT(updateModel()));
    connect(this, SIGNAL(finished(int)), this, SLOT(updateModel()), Qt::QueuedConnection);

    updateModel();
}

addRecord::~addRecord()
{
    delete ui;
}

void addRecord::updateModel()
{
    QStringList* bookList = new QStringList();
    QStringList *bookPageList = new QStringList();

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


    ui->bookLineEdit->setCompleter(completer);
    ui->bookLineEdit->clear();
    ui->sayfaSayisiLineEdit->clear();
    ui->deliveryDateEdit->setDateTime(QDateTime::currentDateTime());
    ui->sonIadeTarihiDateTimeEdit->setDateTime(QDateTime::currentDateTime().addDays(7));
}

void addRecord::on_buttonBox_accepted()
{
    QSqlQuery item;

    item.prepare("INSERT INTO books (book_title, delivery_date, max_return_date, student_id, page)"
                 "VALUES (:bt, :dd, :mrd, :si, :pg)");
    item.bindValue(":bt", ui->bookLineEdit->text().trimmed());
    item.bindValue(":dd", ui->deliveryDateEdit->dateTime().toSecsSinceEpoch());
    item.bindValue(":mrd", ui->sonIadeTarihiDateTimeEdit->dateTime().toSecsSinceEpoch());
    item.bindValue(":si", ui->OgrenciComboBox->currentData().toInt());
    item.bindValue(":pg", ui->sayfaSayisiLineEdit->text().toInt());
    item.exec();
}

void addRecord::on_pushButton_clicked()
{
    st.show();
}
