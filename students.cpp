#include "students.h"
#include "ui_students.h"

#include <QDate>
#include <QFileDialog>
#include <xlsxdocument.h>

students::students(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::students)
{
    ui->setupUi(this);


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

        if(!item.exec()){
            QMessageBox msgbx(QMessageBox::Critical, "Mevcut Kayıt", "Muhtemelen zaten mevcut olan bir <b>öğrenci no</b> ile kayıt etmeyi denediniz<br><br>Hata: " + item.lastError().text());
            msgbx.exec();
        }

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

    studentName = model->index(index.row(), 1).data().toString();

    rbs->connect(rbsui.btnExportStudentRecords, SIGNAL(clicked()), this, SLOT(exportBtnClicked()));

    rbsui.tableView->setModel(recordsModel);
    updateRecordsModel(model->index(index.row(), 0).data().toInt(), studentName);
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

void students::on_pushButton_3_clicked()
{
    const int colCount = model->columnCount();
    const int rowCount = model->rowCount();
    size_t *maxChar = new size_t[colCount]();

    QString s;

    QXlsx::Document exportXls;
    QXlsx::Format bold;
    bold.setFontBold(true);

    QXlsx::Format bordered;
    bordered.setBorderStyle(QXlsx::Format::BorderThin);

    exportXls.addSheet("Ödünç Verilenler");

    /*exportXls.mergeCells(QXlsx::CellRange(1,1,1,colCount));

    exportXls.write(1, 1, "ÖDÜNÇ ALINAN KİTAPLAR");*/

    for(int i=0; i<colCount; i++)
    {
        s = model->headerData(i, Qt::Horizontal).value<QString>();

        if(s.length() > maxChar[i])
            maxChar[i] = s.length();

        exportXls.write(1, i+1, s, bold);
    }

    for(int i=0; i<rowCount; i++)
    {
        for(int j=0; j<colCount; j++)
        {
            s = model->index(i,j).data().toString();

            if(s.length() > maxChar[j])
                maxChar[j] = s.size();

            exportXls.write(i+2, j+1, s,bordered);
        }
    }

    for(int i=0; i<colCount; i++)
        exportXls.setColumnWidth(i+1, maxChar[i]+3);


    QFileDialog fldlg;
    fldlg.setDefaultSuffix("xlsx");

    QString filename = fldlg.getSaveFileName(this, "Dışarıya Aktar", "hilohane_ogrenciler(" + QDate::currentDate().toString(Qt::ISODate) +  ").xlsx", "Excel Dosyası (*.xlsx)");


    if(!filename.isEmpty())
    {
        if(exportXls.saveAs(filename))
        {
#ifdef __linux__
            filename = "xdg-open \"" + filename + "\"";
#elif _WIN32
            filename = "START \""+ filename + "\"";
#endif
            system(filename.toStdString().c_str());
        }

    }
}


// single student report
void students::exportBtnClicked()
{
    const int colCount = recordsModel->columnCount();
    const int rowCount = recordsModel->rowCount();
    size_t *maxChar = new size_t[colCount]();

    QString s;

    QXlsx::Document exportXls;
    QXlsx::Format bold;
    bold.setFontBold(true);

    QXlsx::Format bordered;
    bordered.setBorderStyle(QXlsx::Format::BorderThin);

    exportXls.addSheet(studentName + " - Kütüphane Dökümü");

    /*exportXls.mergeCells(QXlsx::CellRange(1,1,1,colCount));

    exportXls.write(1, 1, "ÖDÜNÇ ALINAN KİTAPLAR");*/

    for(int i=0; i<colCount; i++)
    {
        s = recordsModel->headerData(i, Qt::Horizontal).value<QString>();

        if(s.length() > maxChar[i])
            maxChar[i] = s.length();

        exportXls.write(1, i+1, s, bold);
    }

    for(int i=0; i<rowCount; i++)
    {
        for(int j=0; j<colCount; j++)
        {
            s = recordsModel->index(i,j).data().toString();

            if(s.length() > maxChar[j])
                maxChar[j] = s.size();

            exportXls.write(i+2, j+1, s,bordered);
        }
    }

    for(int i=0; i<colCount; i++)
        exportXls.setColumnWidth(i+1, maxChar[i]+3);


    QFileDialog fldlg;
    fldlg.setDefaultSuffix("xlsx");

    QString filename = fldlg.getSaveFileName(this, "Dışarıya Aktar", studentName + "(" + QDate::currentDate().toString(Qt::ISODate) +  ").xlsx", "Excel Dosyası (*.xlsx)");


    if(!filename.isEmpty())
    {
        if(exportXls.saveAs(filename))
        {
#ifdef __linux__
            filename = "xdg-open \"" + filename + "\"";
#elif _WIN32
            filename = "START \""+ filename + "\"";
#endif
            system(filename.toStdString().c_str());
        }

    }
}
