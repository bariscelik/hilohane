#include "home.h"
#include "ui_home.h"
#include <QModelIndex>
#include <QDir>
#include <QDateTime>
#include <QDateTimeEdit>
#include "xlsxdocument.h"
#include <QFileDialog>
#include "hiloheaderview.h"
#include <QTextCodec>


Home::Home(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Home)
{
    ui->setupUi(this);

    // ************************ //
    // **** CONNECT EVENTS **** //
    // ************************ //
    connect(&ar, SIGNAL(accepted()), this, SLOT(updateModel()), Qt::QueuedConnection);
    connect(&er, SIGNAL(accepted()), this, SLOT(updateModel()), Qt::QueuedConnection);
    connect(&er, SIGNAL(accepted()), this, SLOT(updateReturnedModel()), Qt::QueuedConnection);
    connect(&tl, SIGNAL(updateModel()), this, SLOT(updateModel()), Qt::QueuedConnection);
    connect(&tl, SIGNAL(updateModel()), this, SLOT(updateReturnedModel()), Qt::QueuedConnection);

    // **************************** //
    // **** BOOKS TABLE HEADER **** //
    // **************************** //
    HiloHeaderView *myHeader = new HiloHeaderView(Qt::Horizontal, ui->tableView);

    QLineEdit *filterStudentName = new QLineEdit(myHeader);
    QLineEdit *filterBook = new QLineEdit(myHeader);
    QLineEdit *filterClass = new QLineEdit(myHeader);
    QLineEdit *filterPageNumber = new QLineEdit(myHeader);


    filterStudentName->setClearButtonEnabled(true);
    filterBook->setClearButtonEnabled(true);
    filterClass->setClearButtonEnabled(true);
    filterPageNumber->setClearButtonEnabled(true);


    filterClass->setInputMask("9->A");
    QRegExp rx("(|>|<|\\.|[0-9]){30}");
    filterPageNumber->setValidator(new QRegExpValidator(rx, this));


    QHBoxLayout *deliveryDateRange = new QHBoxLayout;
    QHBoxLayout *maxReturnDateRange = new QHBoxLayout;

    deliveryDateRange->setContentsMargins(0,0,0,0);
    maxReturnDateRange->setContentsMargins(0,0,0,0);

    QDateTimeEdit *filterDeliveryDateStart = new QDateTimeEdit();
    filterDeliveryDateStart->setCalendarPopup(true);

    QDateTimeEdit *filterDeliveryDateEnd = new QDateTimeEdit();
    filterDeliveryDateEnd->setCalendarPopup(true);
    filterDeliveryDateEnd->setDateTime(QDateTime::currentDateTime());

    deliveryDateRange->addWidget(filterDeliveryDateStart);
    deliveryDateRange->addWidget(filterDeliveryDateEnd);

    QDateTimeEdit *filterMaxReturnDateStart = new QDateTimeEdit();
    filterMaxReturnDateStart->setCalendarPopup(true);

    QDateTimeEdit *filterMaxReturnDateEnd = new QDateTimeEdit();
    filterMaxReturnDateEnd->setCalendarPopup(true);
    filterMaxReturnDateEnd->setDateTime(QDateTime::currentDateTime());

    maxReturnDateRange->addWidget(filterMaxReturnDateStart);
    maxReturnDateRange->addWidget(filterMaxReturnDateEnd);



    QGroupBox *deliveryGroup = new QGroupBox(myHeader);
    deliveryGroup->setLayout(deliveryDateRange);
    deliveryGroup->setContentsMargins(0,0,0,0);

    QGroupBox *returnGroup = new QGroupBox(myHeader);
    returnGroup->setLayout(maxReturnDateRange);
    returnGroup->setContentsMargins(0,0,0,0);

    myHeader->widgetList[0] = filterStudentName;
    myHeader->widgetList[1] = filterBook;
    myHeader->widgetList[2] = filterClass;
    myHeader->widgetList[3] = filterPageNumber;
    myHeader->widgetList[4] = deliveryGroup;
    myHeader->widgetList[5] = returnGroup;

    ui->tableView->setHorizontalHeader(myHeader);

    connect(filterStudentName, &QLineEdit::textChanged, [=](const QString &text) {
        m->setRegExpFilter(1, QRegExp(text));
    });

    connect(filterBook, &QLineEdit::textChanged, [=](const QString &text) {
        m->setRegExpFilter(2, QRegExp(text));
    });

    connect(filterClass, &QLineEdit::textChanged, [=](const QString &text) {
        m->setRegExpFilter(3, QRegExp(text));
    });

    connect(filterPageNumber, &QLineEdit::textChanged, [=](const QString &text) {
        m->setNumberFilter(4, text);
    });

    connect(filterDeliveryDateStart, &QDateTimeEdit::dateTimeChanged, [=](const QDateTime &date) {
        m->setDateRangeFilter(5, date.date(), filterDeliveryDateEnd->date());
    });

    connect(filterDeliveryDateEnd, &QDateTimeEdit::dateTimeChanged, [=](const QDateTime &date) {
        m->setDateRangeFilter(5, filterDeliveryDateStart->date(), date.date());
    });

    connect(filterMaxReturnDateStart, &QDateTimeEdit::dateTimeChanged, [=](const QDateTime &date) {
        m->setDateRangeFilter(6, date.date(), filterMaxReturnDateEnd->date());
    });

    connect(filterMaxReturnDateEnd, &QDateTimeEdit::dateTimeChanged, [=](const QDateTime &date) {
        m->setDateRangeFilter(6, filterMaxReturnDateStart->date(), date.date());
    });

    // ******************************* //
    // **** RETURNED TABLE HEADER **** //
    // ******************************* //
    HiloHeaderView *myHeaderR = new HiloHeaderView(Qt::Horizontal, ui->returnedBooksTableView);

    QLineEdit *filterStudentNameR = new QLineEdit(myHeaderR);
    QLineEdit *filterBookR = new QLineEdit(myHeaderR);
    QLineEdit *filterClassR = new QLineEdit(myHeaderR);
    QLineEdit *filterPageNumberR = new QLineEdit(myHeaderR);


    filterStudentNameR->setClearButtonEnabled(true);
    filterBookR->setClearButtonEnabled(true);
    filterClassR->setClearButtonEnabled(true);
    filterPageNumberR->setClearButtonEnabled(true);


    filterClassR->setInputMask("9->A");
    QRegExp rxR("(|>|<|\\.|[0-9]){30}");
    filterPageNumberR->setValidator(new QRegExpValidator(rxR, this));



    QHBoxLayout *deliveryDateRangeR = new QHBoxLayout;
    QHBoxLayout *maxReturnDateRangeR = new QHBoxLayout;

    deliveryDateRangeR->setContentsMargins(0,0,0,0);
    maxReturnDateRangeR->setContentsMargins(0,0,0,0);

    QDateTimeEdit *filterDeliveryDateStartR = new QDateTimeEdit();
    filterDeliveryDateStartR->setCalendarPopup(true);

    QDateTimeEdit *filterDeliveryDateEndR = new QDateTimeEdit();
    filterDeliveryDateEndR->setCalendarPopup(true);
    filterDeliveryDateEndR->setDateTime(QDateTime::currentDateTime());

    deliveryDateRangeR->addWidget(filterDeliveryDateStartR);
    deliveryDateRangeR->addWidget(filterDeliveryDateEndR);

    QDateTimeEdit *filterMaxReturnDateStartR = new QDateTimeEdit();
    filterMaxReturnDateStartR->setCalendarPopup(true);

    QDateTimeEdit *filterMaxReturnDateEndR = new QDateTimeEdit();
    filterMaxReturnDateEndR->setCalendarPopup(true);
    filterMaxReturnDateEndR->setDateTime(QDateTime::currentDateTime());

    maxReturnDateRangeR->addWidget(filterMaxReturnDateStartR);
    maxReturnDateRangeR->addWidget(filterMaxReturnDateEndR);

    QGroupBox *deliveryGroupR = new QGroupBox(myHeaderR);
    deliveryGroupR->setLayout(deliveryDateRangeR);
    deliveryGroupR->setContentsMargins(0,0,0,0);

    QGroupBox *returnGroupR = new QGroupBox(myHeaderR);
    returnGroupR->setLayout(maxReturnDateRangeR);
    returnGroupR->setContentsMargins(0,0,0,0);

    myHeaderR->widgetList[0] = filterStudentNameR;
    myHeaderR->widgetList[1] = filterBookR;
    myHeaderR->widgetList[2] = filterClassR;
    myHeaderR->widgetList[3] = filterPageNumberR;
    myHeaderR->widgetList[4] = deliveryGroupR;
    myHeaderR->widgetList[5] = returnGroupR;

    ui->returnedBooksTableView->setHorizontalHeader(myHeaderR);

    connect(filterStudentNameR, &QLineEdit::textChanged, [=](const QString &text) {
        rm->setRegExpFilter(1, QRegExp(text));
    });

    connect(filterBookR, &QLineEdit::textChanged, [=](const QString &text) {
        rm->setRegExpFilter(2, QRegExp(text));
    });

    connect(filterClassR, &QLineEdit::textChanged, [=](const QString &text) {
        rm->setRegExpFilter(3, QRegExp(text));
    });

    connect(filterPageNumberR, &QLineEdit::textChanged, [=](const QString &text) {
        rm->setNumberFilter(4, text);
    });

    connect(filterDeliveryDateStartR, &QDateTimeEdit::dateTimeChanged, [=](const QDateTime &date) {
        rm->setDateRangeFilter(5, date.date(), filterDeliveryDateEndR->date());
    });

    connect(filterDeliveryDateEndR, &QDateTimeEdit::dateTimeChanged, [=](const QDateTime &date) {
        rm->setDateRangeFilter(5, filterDeliveryDateStartR->date(), date.date());
    });

    connect(filterMaxReturnDateStartR, &QDateTimeEdit::dateTimeChanged, [=](const QDateTime &date) {
        rm->setDateRangeFilter(6, date.date(), filterMaxReturnDateEndR->date());
    });

    connect(filterMaxReturnDateEndR, &QDateTimeEdit::dateTimeChanged, [=](const QDateTime &date) {
        rm->setDateRangeFilter(6, filterMaxReturnDateStartR->date(), date.date());
    });

    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->resizeColumnsToContents();

    updateModel();
    updateReturnedModel();
    mymodel->setSourceModel(model);


    m->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m->setSourceModel(mymodel);

    rm->setFilterCaseSensitivity(Qt::CaseInsensitive);
    rm->setSourceModel(returnedModel);

    ui->tableView->setModel(m);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->resizeColumnsToContents();

    ui->returnedBooksTableView->setModel(rm);
    ui->returnedBooksTableView->setColumnHidden(0,true);
    ui->returnedBooksTableView->resizeColumnsToContents();
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
    ui->returnedBooksTableView->update();
}


void Home::on_action_add_triggered()
{
    ar.show();
    ar.update();
}

void Home::on_action_delete_triggered()
{
    QModelIndexList selection;

    if(iadetab)
    {
        selection = ui->returnedBooksTableView->selectionModel()->selectedRows();
    }else{
        selection = ui->tableView->selectionModel()->selectedRows();
    }

    // delete confirmation name list
    QStringList list;
    QStringList nameList;

    for(int i=0; i< selection.count(); i++)
    {
        list << selection.at(i).data().toString();
        if(iadetab)
        {
            nameList << rm->index(selection.at(i).row(), 1).data().toString() + " - " + rm->index(selection.at(i).row(), 2).data().toString();
        }else{
            nameList << m->index(selection.at(i).row(), 1).data().toString() + " - " + m->index(selection.at(i).row(), 2).data().toString();
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

void Home::on_action_students_triggered()
{
    st.show();
}

void Home::on_action_return_triggered()
{
    QStringList list;
    QModelIndexList selection;
    QString str = "'" + QDateTime::currentDateTime().toString() + "'";
    if(iadetab)
    {
        selection = ui->returnedBooksTableView->selectionModel()->selectedRows();

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

// TODO: setColumnWidth fonksiyonuna auto width özelliği eklenecek
void Home::on_action_export_triggered()
{
    const int colCount = mymodel->columnCount();
    const int rowCount = mymodel->rowCount();
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
        s = mymodel->headerData(i, Qt::Horizontal).value<QString>();

        if(s.length() > maxChar[i])
            maxChar[i] = s.length();

        exportXls.write(1, i+1, s, bold);
    }

    for(int i=0; i<rowCount; i++)
    {
        for(int j=0; j<colCount; j++)
        {
            s = mymodel->index(i,j).data().toString();

            if(s.length() > maxChar[j])
                maxChar[j] = s.size();

            exportXls.write(i+2, j+1, s,bordered);
        }
    }

    for(int i=0; i<colCount; i++)
        exportXls.setColumnWidth(i+1, maxChar[i]+3);


    exportXls.addSheet("İade Edilenler");

    const int rcolCount = returnedModel->columnCount();
    const int rrowCount = returnedModel->rowCount();

    size_t *maxChar2 = new size_t[rcolCount]();


    for(int i=0; i<rcolCount; i++)
    {
        s = returnedModel->headerData(i, Qt::Horizontal).value<QString>();

        if(s.length() > maxChar2[i])
            maxChar2[i] = s.length();

        exportXls.write(1, i+1, s, bold);
    }

    for(int i=0; i<rrowCount; i++)
    {
        for(int j=0; j<rcolCount; j++)
        {
            s = returnedModel->index(i,j).data().toString();

            if(s.length() > maxChar2[j])
                maxChar2[j] = s.size();
            exportXls.write(i+2, j+1, s,bordered);
        }
    }

    for(int i=0; i<colCount; i++)
        exportXls.setColumnWidth(i+1, maxChar2[i]+3);


    QFileDialog fldlg;
    fldlg.setDefaultSuffix("xlsx");

    QString filename = fldlg.getSaveFileName(this, "Dışarıya Aktar", "hilohane_kitaplar(" + QDate::currentDate().toString(Qt::ISODate) +  ").xlsx", "Excel Dosyası (*.xlsx)");


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

void Home::on_tabWidget_currentChanged(int index)
{
    switch (index) {

    // on loan books tab
    case 0:
        iadetab = false;
        ui->action_return->setText("İade Et");
        ui->action_return->setIcon(QIcon(":/assets/icons/ok.png"));
        break;

        // returned books tab
    case 1:
        iadetab=true;
        ui->action_return->setText("Geri Al");
        ui->action_return->setIcon(QIcon(":/assets/icons/return.png"));
        break;
    }
}

void Home::on_tableView_doubleClicked(const QModelIndex &index)
{
    er.setRecordId(m->index(index.row(), 0).data().toInt());
    er.show();
}

void Home::on_returnedBooksTableView_doubleClicked(const QModelIndex &index)
{
    er.setRecordId(rm->index(index.row(), 0).data().toInt());
    er.show();
}

void Home::on_actionTools_triggered()
{
    tl.show();
}
