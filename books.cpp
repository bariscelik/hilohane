#include "books.h"
#include "bookdialog.h"
#include "ui_books.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QDesktopServices>
#include <QPrintDialog>
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QDateTime>

Books::Books(QWidget *parent, int sc_id) :
    QDialog(parent),
    ui(new Ui::Books),
    schoolID(sc_id)
{
    ui->setupUi(this);

    updateModel();

    pModel->setSortLocaleAware(true);
    pModel->setSourceModel( model );

    ui->tblView_booksList->setModel(pModel);
    ui->tblView_booksList->setColumnHidden(0,true);
    ui->tblView_booksList->resizeColumnsToContents();
}

void Books::updateModel()
{
    QSqlQuery query;
    query.prepare("SELECT bookID, title AS 'Başlık', author AS 'Yazar', publisher AS 'Yayınevi', page_number AS 'Sayfa Sayısı', stock AS 'Stok', category AS 'Kategori', publishing_year AS 'Basım Yılı', fixture AS 'Demirbaş No', vol AS 'Cilt No' FROM books WHERE school_id = :sc_id");
    query.bindValue(":sc_id", schoolID);
    query.exec();

    if (query.lastError().isValid())
        QMessageBox::critical(0,"Error",query.lastError().text() );

    model->setQuery(query);
    model->dataChanged(QModelIndex(),QModelIndex());

}

void Books::on_tblView_booksList_doubleClicked(const QModelIndex &index)
{

    BookForm b = {
        pModel->index(index.row(), 0).data().toInt(),
        pModel->index(index.row(), 1).data().toString(),
        pModel->index(index.row(), 2).data().toString(),
        pModel->index(index.row(), 3).data().toString(),
        pModel->index(index.row(), 4).data().toInt(),
        pModel->index(index.row(), 5).data().toInt(),
        pModel->index(index.row(), 6).data().toInt(),
        pModel->index(index.row(), 7).data().toInt(),
        pModel->index(index.row(), 8).data().toInt(),
        pModel->index(index.row(), 9).data().toInt(),
    };

    BookDialog bookDlg(this, b, schoolID);

    if (bookDlg.exec() == QDialog::Accepted)
        updateModel();
}

void Books::showEvent(QShowEvent *e)
{
    updateModel();
}

Books::~Books()
{
    delete ui;
}

void Books::on_btn_add_clicked()
{
    BookDialog bookDlg(this,schoolID);
    if (bookDlg.exec() == QDialog::Accepted)
    {
        ui->tblView_booksList->sortByColumn(0);
        ui->tblView_booksList->horizontalHeader()->setSortIndicator(0, Qt::AscendingOrder);
        updateModel();
        ui->tblView_booksList->scrollToBottom();
        ui->tblView_booksList->selectRow(pModel->rowCount()-1);
    }
}



void Books::on_btn_del_clicked()
{
    QModelIndexList selection = ui->tblView_booksList->selectionModel()->selectedRows();

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
                QSqlQuery query("DELETE FROM books WHERE bookID IN(" + list.join(", ") + ")");

                if (query.lastError().isValid())
                    QMessageBox::critical(0,"Error",query.lastError().text() );

                updateModel();
            }
        }

}

void Books::on_btn_printLabels_clicked()
{

    /*QFileDialog fldlg;
    fldlg.setDefaultSuffix("pdf");


    QString filename = fldlg.getSaveFileName(this, "Etiket Sayfası Hedefi", "etiket.pdf", "Portable Document (*.pdf)");

    if(filename.isEmpty() || filename.isNull())
        return;*/

    if(ui->tblView_booksList->selectionModel()->selectedRows().count() % 36 != 0)
        if(QMessageBox::No == QMessageBox::question(this, "Uyarı", "Seçtiğiniz kayıt sayısı 36'nın katı değil. Etiket kağıdında boşluklar olacak. Yine de devam etmek istiyor musunuz?"))
            return;

    QPageLayout layout(QPageSize(QPageSize::A4), QPageLayout::Portrait, QMarginsF(15,15,15,12), QPageLayout::Millimeter );

    writer = new QPrinter();
    writer->setPageLayout(layout);

    pDlg = new QPrintPreviewDialog(writer);

    pDlg->setWindowTitle("Baskı Önizleme");

    connect(pDlg, SIGNAL(paintRequested(QPrinter *)), this, SLOT(print(QPrinter *)));
    pDlg->open();
}

void Books::print( QPrinter* printer )
{

    QStringList labels;

    QModelIndexList selectedItemsList = ui->tblView_booksList->selectionModel()->selectedRows();

    foreach(QModelIndex index, selectedItemsList)
    {
        QString author, dewey, pubYear, vol;
        dewey = pModel->index(index.row(), 6).data().toString();
        author = pModel->index(index.row(), 2).data().toString().split(" ").last().left(3).toUpper();
        pubYear = pModel->index(index.row(), 7).data().toString();
        vol = QString("k.") + pModel->index(index.row(), 9).data().toString();

        labels << dewey + "\n" + author +   "\n" + pubYear  + "\n" + vol;
    }

    painter = new QPainter(printer);

    painter->setPen(Qt::black);
    painter->setFont(QFont("Arial", 12, QFont::Bold));

    QRect r = painter->viewport();

    QSizeF t(r.width()/4, r.height()/9);

    QPen blackPen;  // creates a default pen

    blackPen.setStyle(Qt::DashDotLine);
    blackPen.setWidth(1);
    blackPen.setBrush(Qt::black);
    blackPen.setCapStyle(Qt::RoundCap);
    blackPen.setJoinStyle(Qt::RoundJoin);

    painter->setPen(blackPen);

    // current label number
    int labelCursor = 0;

    while(labels.count() > labelCursor)
    {
        if(labelCursor != 0)
            printer->newPage();

        for( int i=0; i < 9; i++ )
        {
            for ( int j = 0; j < 4; j++)
            {
                if(labels.count() <= labelCursor)
                    break;

                QRectF p(j*t.width(), i*t.height(),t.width(), t.height());
                //painter.drawRect(p);
                painter->drawText(p, Qt::AlignCenter, labels[labelCursor]);
                labelCursor++;
            }
        }

    }

    painter->end();

}
