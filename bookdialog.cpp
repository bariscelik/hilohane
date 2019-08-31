#include "bookdialog.h"
#include "ui_bookdialog.h"

#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QAbstractItemView>
#include <QPushButton>
#include <QDomDocument>
#include <QUrlQuery>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QCompleter>
#include <QMessageAuthenticationCode>

BookDialog::BookDialog(QWidget *parent, int sc_id) :
    QDialog(parent),
    ui(new Ui::BookDialog),
    schoolID(sc_id)
{
    ui->setupUi(this);
    this->setWindowTitle("Eser Ekle");

    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("Vazgeç");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Ekle");

    ui->comboBox_category->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    getInputs();

}

BookDialog::BookDialog(QWidget *parent, BookForm &f, int sc_id) :
    QDialog(parent),
    ui(new Ui::BookDialog),
    schoolID(sc_id)
{
    ui->setupUi(this);
    this->setWindowTitle("Eser Düzenle");

    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("Vazgeç");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("Kaydet");

    bookID = f.bookID;

    ui->comboBox_category->view()->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    // *********************** //
    // **** SET FORM DATA **** //
    // *********************** //
    getInputs();
    ui->lineEdit_title->setText(f.title);
    ui->lineEdit_author->setText(f.author);
    ui->lineEdit_publisher->setText(f.publisher);
    ui->spinBox_pageNumber->setValue(f.pageNumber);
    ui->spinBox_stock->setValue(f.stock);

    int i = ui->comboBox_category->findData(f.category);
    if ( i != -1 )
        ui->comboBox_category->setCurrentIndex(i);

    ui->spinBox_publishingYear->setValue(f.publishingYear);
    ui->spinBox_fixture->setValue(f.fixture);
    ui->spinBox_vol->setValue(f.vol);
}


BookDialog::~BookDialog()
{
    delete ui;
}

// ************************** //
// **** DEWEY CATEGORIES **** //
// ************************** //
void BookDialog::getInputs()
{
    QSqlQuery dewey("SELECT code,title FROM dewey");

    if (dewey.lastError().isValid())
        QMessageBox::critical(0,"Error",dewey.lastError().text() );

    while(dewey.next())
        ui->comboBox_category->addItem(dewey.value(0).toString() + QString(" - ") + dewey.value(1).toString(),dewey.value(0).toInt());


    QSqlQuery books("SELECT title, author, publisher, (SELECT MAX(fixture) FROM books) AS max_fixture FROM books");
    QStringList bookTitles, bookAuthors, bookPublishers;
    QString tempValue;

    while(books.next())
    {
        tempValue = books.value(0).toString();
        if(!bookTitles.contains(tempValue))
            bookTitles << tempValue;

        tempValue = books.value(1).toString();
        if(!bookAuthors.contains(tempValue))
            bookAuthors << tempValue;

        tempValue = books.value(2).toString();
        if(!bookPublishers.contains(tempValue))
            bookPublishers << tempValue;
    }

    books.first();

    // soft auto-increment fixture
    ui->spinBox_fixture->setValue(books.value(3).toInt() + 1);

    QCompleter* completerTitle = new QCompleter(bookTitles);
    completerTitle->setCaseSensitivity(Qt::CaseInsensitive);
    completerTitle->setFilterMode(Qt::MatchContains);
    ui->lineEdit_title->setCompleter(completerTitle);

    QCompleter* completerAuthor = new QCompleter(bookAuthors);
    completerAuthor->setCaseSensitivity(Qt::CaseInsensitive);
    completerAuthor->setFilterMode(Qt::MatchContains);
    ui->lineEdit_author->setCompleter(completerAuthor);

    QCompleter* completerPublisher = new QCompleter(bookPublishers);
    completerPublisher->setCaseSensitivity(Qt::CaseInsensitive);
    completerPublisher->setFilterMode(Qt::MatchContains);
    ui->lineEdit_publisher->setCompleter(completerPublisher);

    ui->comboBox_category->completer()->setFilterMode(Qt::MatchContains | Qt::MatchRegExp);
    ui->comboBox_category->completer()->setCaseSensitivity(Qt::CaseInsensitive);
    ui->comboBox_category->completer()->setCompletionMode(QCompleter::PopupCompletion);
}

void BookDialog::on_buttonBox_accepted()
{
    QSqlQuery item;

    if(bookID == 0)
    {
        item.prepare("INSERT INTO books(title, author, publisher, page_number, stock, category, publishing_year, fixture, vol, school_id)"
                     "VALUES (:title, :author, :publisher, :page_num, :stock, :category, :publishing_year, :fixture, :vol, :school_id)");

        item.bindValue(":title", ui->lineEdit_title->text().trimmed());
        item.bindValue(":author", ui->lineEdit_author->text().trimmed());
        item.bindValue(":publisher", ui->lineEdit_publisher->text().trimmed());
        item.bindValue(":page_num", ui->spinBox_pageNumber->value());
        item.bindValue(":stock", ui->spinBox_stock->value());
        item.bindValue(":category", ui->comboBox_category->currentData().toInt());
        item.bindValue(":publishing_year", ui->spinBox_publishingYear->value());
        item.bindValue(":fixture", ui->spinBox_fixture->value());
        item.bindValue(":vol", ui->spinBox_vol->value());
        item.bindValue(":school_id", schoolID );

    }else{
        item.prepare("UPDATE books SET title = :title, author = :author, publisher = :publisher, page_number = :page_number, stock = :stock, category = :category, publishing_year = :publishing_year, fixture = :fixture, vol = :vol WHERE bookID = :book_id");

        item.bindValue(":title", ui->lineEdit_title->text().trimmed());
        item.bindValue(":author", ui->lineEdit_author->text().trimmed());
        item.bindValue(":publisher", ui->lineEdit_publisher->text().trimmed());
        item.bindValue(":page_number", ui->spinBox_pageNumber->value());
        item.bindValue(":stock", ui->spinBox_stock->value());
        item.bindValue(":category", ui->comboBox_category->currentData().toInt());
        item.bindValue(":publishing_year", ui->spinBox_publishingYear->value());
        item.bindValue(":fixture", ui->spinBox_fixture->value());
        item.bindValue(":vol", ui->spinBox_vol->value());
        item.bindValue(":book_id", bookID);
    }

    item.exec();

    if (item.lastError().isValid())
        QMessageBox::critical(0,"Error",item.lastError().text());
}
/*
void BookDialog::on_lineEdit_title_textChanged(const QString &arg1)
{

    QString AWSAccessKeyID = "SECRET";
    QString AWSSecretKey   = "SECRET";

    QUrlQuery uq;
    QList<QPair<QString,QString>> queryList;
    queryList
            << QPair<QString,QString>("Service","AWSECommerceService")
            << QPair<QString,QString>("Operation","ItemSearch")
            << QPair<QString,QString>("SubscriptionId",AWSAccessKeyID)
            << QPair<QString,QString>("AssociateTag","")
            << QPair<QString,QString>("SearchIndex","All")
            << QPair<QString,QString>("ResponseGroup","Images,ItemAttributes,Offers");

    uq.setQueryItems(queryList);


    QDomDocument x;
    QString url = "https://webservices.amazon.com/onca/xml?" + uq.query();

    QString signature = "GET\nwebservices.amazon.com\n/onca/xml";
    QString signedUrl(QMessageAuthenticationCode::hash(url.toLatin1(), AWSSecretKey.toLatin1(), QCryptographicHash::Sha256));


    QNetworkAccessManager manager;
    QNetworkReply *response = manager.get(QNetworkRequest(QUrl(signedUrl)));
    QEventLoop event;
    connect(response,SIGNAL(finished()),&event,SLOT(quit()));
    event.exec();
    QString html = response->readAll(); // Source should be stored here

    qDebug() << html;

    x.setContent(html);


}*/
