#ifndef BOOKS_H
#define BOOKS_H

#include <QDialog>
#include <QPainter>
#include <QPrintPreviewDialog>
#include <QPrinter>
#include <QSortFilterProxyModel>
#include <QSqlQueryModel>

namespace Ui {
class Books;
}

class Books : public QDialog
{
    Q_OBJECT

public:
    explicit Books(QWidget *parent = nullptr, int sc_id = 0);
    ~Books();
private slots:
    void updateModel();
    void on_tblView_booksList_doubleClicked(const QModelIndex &index);
    void on_btn_add_clicked();

    void on_btn_del_clicked();

    void on_btn_printLabels_clicked();

    void print(QPrinter *printer);
private:
    void showEvent(QShowEvent *e);
    Ui::Books *ui;
    QSqlQueryModel *model = new QSqlQueryModel();
    QSortFilterProxyModel *pModel = new QSortFilterProxyModel();
    int schoolID;
    QPrinter *writer;
    QPainter *painter;
    QPrintPreviewDialog *pDlg;
};

#endif // BOOKS_H
