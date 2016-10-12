#include "students.h"
#include "ui_students.h"

students::students(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::students)
{
    ui->setupUi(this);


    createConnection();

    updateModel();

    ui->tableView->setModel(model);
    ui->tableView->setColumnHidden(0,true);

}

void students::updateModel()
{
    QSqlQuery query("SELECT students.id, students.name as 'İsim', students.number as 'Okul Numarası', students.class as 'Sınıfı', COUNT(books.id) as 'Kitap Sayısı', SUM(books.page) as 'Okuduğu Sayfa Sayısı' FROM students "
                    "LEFT OUTER JOIN books ON books.student_id = students.id GROUP BY students.id");
    if (query.lastError().isValid()) QMessageBox::critical(0,"Error",query.lastError().text() );

    model->setQuery(query);
    model->dataChanged(QModelIndex(),QModelIndex());
}

void students::showEvent(QShowEvent *e)
{
    updateModel();
}

students::~students()
{
    delete ui;
}

void students::on_pushButton_clicked()
{
    QDialog *adst = new QDialog;
    adstui.setupUi(adst);

    adst->layout()->setSizeConstraint( QLayout::SetFixedSize );
    adstui.buttonBox->button(QDialogButtonBox::Cancel)->setText("Vazgeç");
    adstui.buttonBox->button(QDialogButtonBox::Ok)->setText("Ekle");

    adstui.okulNumarasiLineEdit->setValidator(new QIntValidator(0, 1000, this));
    connect(adstui.buttonBox, &QDialogButtonBox::accepted, [=]() {
        QSqlQuery item;
        item.prepare("INSERT INTO students (name, number, class)"
                     "VALUES (:name, :number, :class)");
        item.bindValue(":name", adstui.isimLineEdit->text());
        item.bindValue(":number", adstui.okulNumarasiLineEdit->text());
        item.bindValue(":class", adstui.sinifLineEdit->text());
        item.exec();
        updateModel();
    });

    adst->show();
}

void students::on_pushButton_2_clicked()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();

    QStringList list;
    QStringList nameList;

    for(int i=0; i< selection.count(); i++)
    {
        list << selection.at(i).data().toString();
        nameList << model->index(selection.at(i).row(), 1).data().toString();
    }

    QMessageBox msgbx(QMessageBox::Question, "Silme Onayı", "Aşağıdaki (seçili) kayıtları silmek istediğinize emin misiniz?\n" + nameList.join("\n"), QMessageBox::Yes|QMessageBox::No);
    msgbx.setButtonText(QMessageBox::Yes, tr("Evet"));
    msgbx.setButtonText(QMessageBox::No, tr("Hayır"));

    if(list.size() > 0){
            if(QMessageBox::Yes == msgbx.exec())
            {
                QSqlQuery("DELETE FROM students WHERE id IN(" + list.join(", ") + ")");
                updateModel();
            }
        }
    }

void students::on_tableView_doubleClicked(const QModelIndex &index)
{
    QDialog *rbs = new QDialog;
    rbsui.setupUi(rbs);

    rbsui.tableView->setModel(recordsModel);
    updateRecordsModel(model->index(index.row(), 0).data().toInt(), model->index(index.row(), 1).data().toString());
    rbsui.tableView->setColumnHidden(0,true);
    rbsui.tableView->resizeColumnsToContents();
    rbs->show();

}

void students::updateRecordsModel(int student_id, QString student_name)
{
    QSqlQuery studentsQuery("SELECT name, number, class FROM students WHERE id =" + QString::number(student_id) + " LIMIT 1");
    if (studentsQuery.lastError().isValid()) QMessageBox::critical(0,"Error",studentsQuery.lastError().text() );
    studentsQuery.first();
    rbsui.studentInfoLabel->setText("<b>Adı&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;:</b> " + studentsQuery.value(0).toString() + "<br><b>Numarası&nbsp;:</b> " + studentsQuery.value(1).toString() + "<br><b>Sınıfı&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;:</b> " + studentsQuery.value(2).toString());
    QSqlQuery booksQuery("SELECT id as '#ID', book_title as 'Kitap Adı', page as 'Sayfa Sayısı', delivery_date as 'Veriliş Tarihi', max_return_date as 'Son İade Tarihi' FROM books WHERE books.student_id =" + QString::number(student_id));
    if (booksQuery.lastError().isValid()) QMessageBox::critical(0,"Error",booksQuery.lastError().text() );

    recordsModel->setQuery(booksQuery);
    recordsModel->dataChanged(QModelIndex(),QModelIndex());
    rbsui.tableView->update();
}
