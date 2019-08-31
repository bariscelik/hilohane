#ifndef EDITBOOKDIALOG_H
#define EDITBOOKDIALOG_H

#include <QDialog>

namespace Ui {
class BookDialog;
}

typedef struct {
    int bookID;
    QString title,
            author,
            publisher;

    int pageNumber,
        stock,
        category,
        publishingYear,
        fixture,
        vol;
}BookForm;

class BookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BookDialog(QWidget *parent, int sc_id);
    BookDialog(QWidget *parent, BookForm &f, int sc_id);
    ~BookDialog();

private:
    Ui::BookDialog *ui;
    int bookID = 0;
    int schoolID;
    // QDialog interface
    void getInputs();
private slots:
    void on_buttonBox_accepted();
    //void on_lineEdit_title_textChanged(const QString &arg1);
};

#endif // EDITBOOKDIALOG_H
