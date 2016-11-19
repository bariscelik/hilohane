#include "home.h"
#include "ui_home.h"
#include <QModelIndex>
#include <QDir>
#include <QDateTime>
#include <QSortFilterProxyModel>
#include "xlsxdocument.h"
#include <QFileDialog>

Home::Home(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Home)
{
    ui->setupUi(this);

    connect(&ar, SIGNAL(accepted()), this, SLOT(updateModel()), Qt::QueuedConnection);
    connect(&er, SIGNAL(accepted()), this, SLOT(updateModel()), Qt::QueuedConnection);
    connect(&er, SIGNAL(accepted()), this, SLOT(updateReturnedModel()), Qt::QueuedConnection);

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->resizeColumnsToContents();



    updateModel();
    updateReturnedModel();
    mymodel->setSourceModel(model);

    QSortFilterProxyModel *m=new QSortFilterProxyModel(this);
    m->setDynamicSortFilter(true);
    m->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m->setSourceModel(mymodel);

    ui->tableView->setModel(m);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->resizeColumnsToContents();

    QSortFilterProxyModel *rm=new QSortFilterProxyModel(this);
    rm->setDynamicSortFilter(true);
    rm->setFilterCaseSensitivity(Qt::CaseInsensitive);
    rm->setSourceModel(returnedModel);

    ui->iadeTableView->setModel(rm);
    ui->iadeTableView->setColumnHidden(0,true);
    ui->iadeTableView->resizeColumnsToContents();
}

Home::~Home()
{
    delete ui;
}

void Home::updateModel()
{
    QSqlQuery query("SELECT books.id as '#ID', students.name as 'Öğrenci', books.book_title as 'Kitap Adı', students.class as 'Sınıfı', books.page as 'Sayfa Sayısı', books.delivery_date as 'Veriliş Tarihi', books.max_return_date as 'Son İade Tarihi' FROM books LEFT OUTER JOIN students ON students.id = books.student_id WHERE books.return_date=books.delivery_date");
    if (query.lastError().isValid()) QMessageBox::critical(0,"Error",query.lastError().text() );

    model->setQuery(query);
    model->dataChanged(QModelIndex(),QModelIndex());
    ui->tableView->update();
}

void Home::updateReturnedModel()
{
    QSqlQuery query("SELECT books.id as '#ID', students.name as 'Öğrenci', books.book_title as 'Kitap Adı', students.class as 'Sınıfı', books.page as 'Sayfa Sayısı', books.delivery_date as 'Veriliş Tarihi', books.max_return_date as 'Son İade Tarihi', books.return_date as 'İade Tarihi' FROM books LEFT OUTER JOIN students ON students.id = books.student_id WHERE books.return_date <> books.delivery_date");
    if (query.lastError().isValid()) QMessageBox::critical(0,"Error",query.lastError().text() );

    returnedModel->setQuery(query);
    returnedModel->dataChanged(QModelIndex(),QModelIndex());
    ui->iadeTableView->update();
}


void Home::on_actionAdd_triggered()
{
    ar.show();
    ar.update();
}

void Home::on_actionDelete_triggered()
{
    QModelIndexList selection;

    if(iadetab)
    {
        selection = ui->iadeTableView->selectionModel()->selectedRows();
    }else{
        selection = ui->tableView->selectionModel()->selectedRows();
    }

    QStringList list;
    QStringList nameList;

    for(int i=0; i< selection.count(); i++)
    {
        list << selection.at(i).data().toString();
        if(iadetab)
        {
            nameList << returnedModel->index(selection.at(i).row(), 1).data().toString() + " - " + returnedModel->index(selection.at(i).row(), 2).data().toString();
        }else{
            nameList << model->index(selection.at(i).row(), 1).data().toString() + " - " + model->index(selection.at(i).row(), 2).data().toString();
        }
    }

    QMessageBox msgbox(QMessageBox::Question, "Silme Onayı", "Aşağıdaki kayıtları silmek istediğinize emin misiniz?\n" + nameList.join("\n"), QMessageBox::Yes|QMessageBox::No);
    msgbox.setButtonText(QMessageBox::Yes, tr("Evet"));
    msgbox.setButtonText(QMessageBox::No, tr("Hayır"));

    if(list.size() > 0){
        if(QMessageBox::Yes == msgbox.exec())
        {
            QSqlQuery("DELETE FROM books WHERE id IN(" + list.join(", ") + ")");
            updateModel();
            updateReturnedModel();
        }else{
            ui->tableView->clearSelection();
        }
    }
}

QString Home::getLastExecutedQuery(const QSqlQuery& query)
{
 QString str = query.lastQuery();
 QMapIterator<QString, QVariant> it(query.boundValues());
 while (it.hasNext())
 {
  it.next();
  str.replace(it.key(),it.value().toString());
 }
 return str;
}

void Home::on_action_ogrenciler_triggered()
{
    st.show();
}

void Home::on_action_iade_triggered()
{
    QStringList list;
    QModelIndexList selection;
    QString str = "'" + QDateTime::currentDateTime().toString() + "'";
    if(iadetab)
    {
        selection = ui->iadeTableView->selectionModel()->selectedRows();

        for(int i=0; i< selection.count(); i++)
        {
            list << selection.at(i).data().toString();
        }

        str = "delivery_date";

    }else{
        selection = ui->tableView->selectionModel()->selectedRows();

        for(int i=0; i< selection.count(); i++)
        {
            list << selection.at(i).data().toString();
        }
    }

    if(list.size() > 0){
            QSqlQuery("UPDATE books SET return_date = " + str + " WHERE id IN(" + list.join(", ") + ")");
            updateModel();
            updateReturnedModel();
    }
}

void Home::on_actionExport_triggered()
{
    QXlsx::Document exportXls;
    QXlsx::Format bold;
    bold.setFontBold(true);

    exportXls.addSheet("Ödünç Verilenler");

    int colCount = mymodel->columnCount();
    int rowCount = mymodel->rowCount();

    for(int i=0; i<colCount; i++)
    {
        exportXls.write(1, i+1, mymodel->headerData(i, Qt::Horizontal).value<QString>(), bold);
    }

    for(int i=0; i<rowCount; i++)
    {
        for(int j=0; j<colCount; j++)
        {
            exportXls.write(i+2, j+1, mymodel->index(i,j).data().toString());
        }
    }

    exportXls.addSheet("İade Edilenler");

    int rcolCount = returnedModel->columnCount();
    int rrowCount = returnedModel->rowCount();

    for(int i=0; i<rcolCount; i++)
    {
        exportXls.write(1, i+1, returnedModel->headerData(i, Qt::Horizontal).value<QString>(), bold);
    }

    for(int i=0; i<rrowCount; i++)
    {
        for(int j=0; j<rcolCount; j++)
        {
            exportXls.write(i+2, j+1, returnedModel->index(i,j).data().toString());
        }
    }
    QFileDialog fldlg;
    fldlg.setDefaultSuffix("xlsx");

    QString filename = fldlg.getSaveFileName(this, "Dışarıya Aktar", "hilohane.xlsx", "Excel Dosyası (*.xlsx)");


    if(!filename.isEmpty())
    {
        exportXls.saveAs(filename);
    }

}

void Home::on_tabWidget_currentChanged(int index)
{
    if(index == 1){
        iadetab=true;
        ui->action_iade->setText("Geri Al");
        ui->action_iade->setIcon(QIcon(":/assets/icons/return.png"));
    }else{
        iadetab = false;
        ui->action_iade->setText("İade Et");
        ui->action_iade->setIcon(QIcon(":/assets/icons/ok.png"));
    }
}

void Home::on_tableView_doubleClicked(const QModelIndex &index)
{
    er.setRecordId(model->index(index.row(), 0).data().toInt());
    er.show();
}

void Home::on_iadeTableView_doubleClicked(const QModelIndex &index)
{
    er.setRecordId(returnedModel->index(index.row(), 0).data().toInt());
    er.show();
}
