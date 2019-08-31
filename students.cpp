#include "studentdialog.h"
#include "students.h"
#include "ui_students.h"

#include <QDate>
#include <QFileDialog>
#include <xlsxdocument.h>

students::students(QWidget *parent, int sc_id) :
    QDialog(parent),
    ui(new Ui::students),
    schoolID(sc_id)
{
    ui->setupUi(this);


    updateModel();

   pModel->setSortLocaleAware(true);
   pModel->setSourceModel( model );

    ui->tableView->setModel(pModel);
    ui->tableView->setColumnHidden(0,true);
}

void students::updateModel()
{
    QSqlQuery query;
    query.prepare("SELECT students.id, students.name as 'İsim', students.number as 'Okul Numarası', students.class as 'Sınıfı', COUNT(loans.loanID) as 'Kitap Sayısı', SUM(books.page_number) as 'Okuduğu Sayfa Sayısı' FROM students "
                  "LEFT OUTER JOIN loans ON loans.student_id = students.id "
                  "LEFT OUTER JOIN books ON books.bookID = loans.book_id "
                  "WHERE students.school_id = :sc_id GROUP BY students.id");
    query.bindValue(":sc_id", schoolID);
    query.exec();

    if (query.lastError().isValid())
        QMessageBox::critical(0,"Error",query.lastError().text() );

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
    StudentDialog stDialog(this, schoolID);

    if (stDialog.exec() == QDialog::Accepted)
        updateModel();
}

void students::on_pushButton_2_clicked()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();

    QStringList list;
    QStringList nameList;

    for(int i=0; i< selection.count(); i++)
    {
        list << selection.at(i).data().toString();
        nameList << pModel->index(selection.at(i).row(), 1).data().toString();
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

    StudentForm s{
        pModel->index(index.row(), 0).data().toInt(),
        pModel->index(index.row(), 1).data().toString(),
        pModel->index(index.row(), 2).data().toInt(),
        pModel->index(index.row(), 3).data().toString(),
    };

    StudentDialog stDialog(this, s, schoolID);

    if (stDialog.exec() == QDialog::Accepted)
        updateModel();

}

void students::updateRecordsModel()
{
    QSqlQuery studentsQuery("SELECT name, number, class FROM students WHERE id =" + QString::number(currentStudentID) + " LIMIT 1");
    if (studentsQuery.lastError().isValid()) QMessageBox::critical(0,"Error",studentsQuery.lastError().text());
    studentsQuery.first();
    rbsui.studentInfoLabel->setText("<b>Adı&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;:</b> " + studentsQuery.value(0).toString() + "<br><b>Numarası&nbsp;:</b> " + studentsQuery.value(1).toString() + "<br><b>Sınıfı&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;:</b> " + studentsQuery.value(2).toString());
    QSqlQuery bq;
    bq.prepare("SELECT books.bookID as '#ID', books.title as 'Kitap Adı', books.page_number as 'Sayfa Sayısı', strftime('%d.%m.%Y %H:%M', datetime(loans.delivery_date, "
               " 'unixepoch')) as 'Veriliş Tarihi', strftime('%d.%m.%Y %H:%M', datetime(loans.return_date, 'unixepoch')) as 'İade Tarihi' FROM loans"
               " LEFT OUTER JOIN books ON books.bookID = loans.book_id"
               " WHERE student_id = :st_id");

    bq.bindValue(":st_id", currentStudentID);
    bq.exec();

    if (bq.lastError().isValid())
        QMessageBox::critical(0,"Error",bq.lastError().text() );

    recordsModel->setQuery(bq);
    recordsModel->dataChanged(QModelIndex(),QModelIndex());
    rbsui.tableView->update();
}

void students::on_pushButton_3_clicked()
{
    const int colCount = model->columnCount();
    const int rowCount = model->rowCount();

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
        exportXls.write(1, i+1, model->headerData(i, Qt::Horizontal).toString(), bold);
    }

    for(int i=0; i<rowCount; i++)
    {
        for(int j=0; j<colCount; j++)
        {
            exportXls.write(i+2, j+1, model->index(i,j).data().toString(),bordered);
        }
    }

    exportXls.currentWorksheet()->setAutoColumnWidth(1, colCount,rowCount);

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

    QXlsx::Document exportXls;
    QXlsx::Format bold;
    bold.setFontBold(true);

    QXlsx::Format bordered;
    bordered.setBorderStyle(QXlsx::Format::BorderThin);

    exportXls.addSheet(studentName + " - Kütüphane Dökümü");

    /*exportXls.mergeCells(QXlsx::CellRange(1,1,1,colCount));

    exportXls.write(1, 1, "ÖDÜNÇ ALINAN KİTAPLAR");*/

    QSqlQuery studentsQuery("SELECT name, number, class FROM students WHERE id =" + QString::number(currentStudentID) + " LIMIT 1");
    if (studentsQuery.lastError().isValid()) QMessageBox::critical(0,"Error",studentsQuery.lastError().text() );
    studentsQuery.first();

    exportXls.write(1, 1, "Adı", bold);
    exportXls.write(1, 2, studentsQuery.value(0).toString());

    exportXls.write(2, 1, "Sınıfı", bold);
    exportXls.write(2, 2, studentsQuery.value(2).toString());

    exportXls.write(3, 1, "Numarası", bold);
    exportXls.write(3, 2, studentsQuery.value(1).toString());


    for(int i=0; i<colCount; i++)
    {

        exportXls.write(4, i+1, recordsModel->headerData(i, Qt::Horizontal).toString(), bold);
    }

    for(int i=0; i<rowCount; i++)
    {
        for(int j=0; j<colCount; j++)
        {
            exportXls.write(i+5, j+1, recordsModel->index(i,j).data().toString(),bordered);
        }
    }

    exportXls.currentWorksheet()->setAutoColumnWidth(1, colCount,rowCount);

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

void students::on_pushButton_studentDetail_clicked()
{
    if(ui->tableView->selectionModel()->selectedIndexes().count()==0)
        return;

    QDialog *rbs = new QDialog;
    rbsui.setupUi(rbs);

    studentName = pModel->index(ui->tableView->selectionModel()->selectedIndexes().first().row(), 1).data().toString();


    rbs->connect(rbsui.btnExportStudentRecords, SIGNAL(clicked()), this, SLOT(exportBtnClicked()));

    currentStudentID = pModel->index(ui->tableView->selectionModel()->selectedIndexes().first().row(), 0).data().toInt();

    rbsui.tableView->setModel(recordsModel);
    updateRecordsModel();
    rbsui.tableView->setColumnHidden(0,true);
    rbsui.tableView->resizeColumnsToContents();
    rbs->show();
}
